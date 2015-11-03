#ifndef __ANDROID_PLATFORM_H__
#define __ANDROID_PLATFORM_H__

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#define StartUpNetService()

#define SOCKET                int
#define SOCKET_ERROR          -1
#define closesocket( socket ) close( socket )

#define NonBlock( socket )    int flags = fcntl(socket, F_GETFL, 0);\
                              fcntl(socket, flags|O_NONBLOCK);
	
#define TIMEVAL               struct timeval


#endif
