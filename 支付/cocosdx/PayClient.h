#ifndef __PAY_CLIENT_THREAD_H__
#define __PAY_CLIENT_THREAD_H__

#include "platform.h"
#include "Log.h"

class PayClient
{
public:
	PayClient(void);
	~PayClient(void);
	static PayClient* GetInstance();
public:
	int Init();

	int Pay( double price, string thing, string description );
	int Check( string &result );
	int Query( string orderID, bool &statu );

public:
	void SendPacket( char *req );
	void RecvPacket( char *rsp );
private:
	SOCKET m_socket;
	CLog *m_log;

	static PayClient *m_instance;
};


#endif
