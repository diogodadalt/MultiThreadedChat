#ifndef SOCKET_WRAPPER_H
#define SOCKET_WRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct ConnectionInfo {
	int sockfd;
	struct sockaddr_in address;
	void* additionalInfo;
} ConnectionInfo;

void connectServerBlocking(int port, int maxConns, void* (*socketHandler)(void*), void* additionalInfo);
void connectClientBlocking(char* host, int port, void* (*socketHandler)(void*), void* additionalInfo);

#endif