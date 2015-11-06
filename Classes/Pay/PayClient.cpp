#include "PayClient.h"

PayClient * PayClient::m_instance = new PayClient;

PayClient::PayClient(void):m_socket( SOCKET_ERROR ),m_log(NULL)
{
}


PayClient::~PayClient(void)
{
	if( m_log != NULL )
	{
		m_log->M_Log( "PayClient Release!" );
		delete m_log;
		m_log = NULL;
	}
}

PayClient* PayClient::GetInstance()
{
	return m_instance;
}

int PayClient::Init()
{
	//创建socket
	m_log = new CLog("pay_client.log");
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if( m_socket == SOCKET_ERROR )
	{
		StartUpNetService();
		m_socket = socket( AF_INET, SOCK_STREAM, 0 );
		if( m_socket == SOCKET_ERROR )
		{
			m_log->M_Log( "Socket Creat Fail!" );
			return -1;
		}
	}
	NonBlock( m_socket );
	m_log->M_Log( "Socket Creat Succeed!" );

	//创建连接
	sockaddr_in addrServer; 
	addrServer.sin_family      = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	addrServer.sin_port        = htons( 10812 );

	m_log->M_Log( "Ready to Connect!" );
	if( 0 != connect( m_socket, (struct sockaddr*) &addrServer, sizeof(addrServer) ) )
	{
		closesocket( m_socket );
		m_socket = SOCKET_ERROR;

		m_log->M_Log( "Connect Fail!" );
		return -2;
	}

	m_log->M_Log( "Connect Succeed!" );
	return 1;
}

int PayClient::Pay(double price, string thing, string description )
{
	m_log->M_Log( "Pay: %lf,%s,%s!", price, thing.c_str(), description.c_str() );

	char send_buffer[1024];
	char recv_buffer[1024];

	memset( send_buffer, 0, sizeof( send_buffer ) );
	memset( recv_buffer, 0, sizeof( recv_buffer ) );

	if( price < 0.01 )
		return -1;
	else if( thing.empty() )
		return -2;

	sprintf( send_buffer, "pay zfb,%lf,%s,%s", price, thing.c_str(), description.c_str() );//方式（支付宝（zfb），微信（wx）），价钱，物品，描述

	m_log->M_Log( "Send send_buffer:%s", send_buffer );
	SendPacket( send_buffer );
//	m_log->M_Log( "Wait RecvPacket" );
//	RecvPacket( recv_buffer );

	m_log->M_Log( "Recv Result recv_buffer:%s",recv_buffer );
	//result = recv_buffer;
	return 1;
}


int PayClient::Check( string &result )
{
	char recv_buffer[1024] = "\0";
	RecvPacket( recv_buffer );
	result = recv_buffer;
	m_log->M_Log( "Recv Result recv_buffer:%s",recv_buffer );
	return 1;
}

int PayClient::Query(string orderID, bool &succeed )
{
	m_log->M_Log( "Query: %s!", orderID.c_str() );

	char send_buffer[1024];
	char recv_buffer[1024];

	memset( send_buffer, 0, sizeof( send_buffer ) );
	memset( recv_buffer, 0, sizeof( recv_buffer ) );

	if( orderID.empty() == true )
		return -1;

	sprintf( send_buffer, "query %s", orderID.c_str() );//订单号
	m_log->M_Log( "Send send_buffer:%s", send_buffer );
	SendPacket( send_buffer );

	m_log->M_Log( "Wait RecvPacket" );
	RecvPacket( recv_buffer );
	m_log->M_Log( "Recv OrderID Statu recv_buffer:%s",recv_buffer );

	if( strcmp( "NOTPAY", recv_buffer ) == 0 || strcmp( "", recv_buffer ) == 0 )
	{
		succeed = false;
	}
	else
	{
		succeed = true;
	}
	return 1;
}

void PayClient::SendPacket(char *req)
{
	if( m_socket == SOCKET_ERROR )
		return ;

	strcat( req, "\n" );//java的readLine收到回车后才能返回

	int size     = strlen( req );
	int send_len = send( m_socket, req, size, 0 );
}

void PayClient::RecvPacket(char *rsp)
{
	if( m_socket == SOCKET_ERROR )
		return ;
	fd_set fd_read;

	TIMEVAL time_value;
	time_value.tv_sec = 0;
	time_value.tv_usec = 100;

	char recv_buffer[1024];
	char recv_head[1024];

	memset( recv_buffer, 0, sizeof( recv_buffer ) );
	memset( recv_head,   0, sizeof( recv_head ) );

	FD_ZERO(&fd_read);
	FD_SET( m_socket, &fd_read );

	select( m_socket+1, &fd_read, NULL, NULL, &time_value );

	if( FD_ISSET( m_socket, &fd_read) )
	{
		int size = 1024;
		m_log->M_Log( "Recv ready" );
		int recv_len = recv( m_socket, recv_buffer, size, 0 );

		m_log->M_Log( "Recv recv_buffer:%s",recv_buffer );

		int i = 0;
		for( ; i < strlen( recv_buffer ); i++ )
		{
			if( recv_buffer[i] != ' ' )
				recv_head[i] = recv_buffer[i];
			else
				break;
		}

		if( strcmp( recv_head, "orderId" ) == 0 )
		{//下单成功，返回订单号
			strcpy( rsp, recv_buffer+i );
		}
		else if( strcmp( recv_head, "succeed" ) == 0 )
		{//查询成功返回支付状态
			strcpy( rsp, recv_buffer+i );
		}
		else
		{//错误返回支付失败
			strcpy( rsp, recv_buffer+i );
		}
		//break;
	}
}

