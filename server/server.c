#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../common/socket_wrapper.h"
#include "command.h"
#include "client_info.h"

#define PORT 4000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 256

void broadCastMessage(char* message, ClientList* clientList) {
	int i, n;
	pthread_mutex_t clientListMutex = getClientsMutex();
	pthread_mutex_lock(&clientListMutex);
	for (i = 0; i < clientList->size; i++) {
		n = write(clientList->clients[i]->sockfd, message, strlen(message));
		if (n < 0)
			printf("ERROR writing to socket");
	}
	pthread_mutex_unlock(&clientListMutex);
}

void* startClientThread(void* args) {
	ClientInfo* clientInfo = (ClientInfo*)args;
	int n;
	int sockfd = clientInfo->sockfd;
	char buffer[BUFFER_SIZE];
	ClientList* clientList = clientInfo->clientList;
	char* finalMessage = NULL;

	while(1) {
		bzero(buffer, BUFFER_SIZE);

		/* read from the socket */
		n = read(sockfd, buffer, 256);
		if (n < 0)
			printf("ERROR reading from socket");

		int isCommand = processCommandIfExist(buffer, clientInfo);

		if (isCommand) {
			finalMessage = malloc(42);
			strcpy(finalMessage, "Usuario ");
			strcat(finalMessage, clientInfo->userName);
			strcat(finalMessage, " saiu do chat.");
		} else
			finalMessage = assembleMessage(clientInfo, buffer);

		printf("%s\n", finalMessage);
		broadCastMessage(finalMessage, clientList);
		free(finalMessage);

		if (isCommand) {
			close(clientInfo->sockfd);
			break;
		}
	}

	return 0;
}

void* handleClientConnection(void* args) {
	ConnectionInfo* connInfo = (ConnectionInfo*)args;
	ClientList* clientList = (ClientList*)connInfo->additionalInfo;
	pthread_t clientThread;
	ClientInfo* ci = (ClientInfo*)malloc(sizeof(ClientInfo));
	ci->sockfd = connInfo->sockfd;
	ci->address = connInfo->address;
	ci->clientList = clientList;
	addClient(clientList, ci, MAX_CLIENTS);
	sprintf(ci->userName, "anonymous_%d", ci->address.sin_port);
	printf("Usuario %s se conectou: \n", ci->userName);
	pthread_create(&clientThread, NULL, startClientThread, ci);
	return 0;
}

int main(void) {
	ClientList* clientList = malloc(sizeof(ClientList));
	clientList->clients = calloc(MAX_CLIENTS, sizeof(ClientInfo*));
	clientList->size = 0;

	connectServerBlocking(PORT, MAX_CLIENTS, handleClientConnection, clientList);

	return 0;
}