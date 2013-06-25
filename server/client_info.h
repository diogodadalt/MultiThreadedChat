#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct ClientInfo {
	int sockfd;
	char userName[20];
	struct sockaddr_in address;
	struct ClientList* clientList;
} ClientInfo;

typedef struct ClientList {
	ClientInfo** clients;
	int size;
} ClientList;

void addClient(ClientList* clientList, ClientInfo* clientInfo, int maxClients);
void removeClient(ClientList* clientList, ClientInfo* clientInfo);
pthread_mutex_t getClientsMutex();
void printClient(ClientInfo* clientInfo);
void printClients(ClientList* clientList);
char* assembleMessage(ClientInfo* clientInfo, char* message);

#endif