#ifndef __WIN32PLATFORM_H__
#define __WIN32PLATFORM_H__

#include <winsock2.h>


#define StartUpNetService() WSADATA wsaData;\
							WSAStartup(MAKEWORD(2, 2), &wsaData)


#define closesocket( socket ) closesocket( socket )

#define NonBlock( socket )    unsigned long nonblock_mode = 1;\
							  ioctlsocket( socket, FIONBIO, &nonblock_mode)

#endif