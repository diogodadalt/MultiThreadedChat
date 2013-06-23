#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 4000
#define MAX_CLIENTS 100

typedef struct ClientInfo {
	int sockfd;
	char userName[20];
	struct sockaddr_in address;
} ClientInfo;

typedef struct ClientList {
	ClientInfo** clients;
	int size;
} ClientList;

typedef struct SocketInfo {
	char* buffer;
	int newsockfd;
	ClientList* clientList;
	ClientInfo* currentClient;
} SocketInfo;

void broadCastMessage(char* message, ClientList* clientList) {
	int i, n;	
	for (i = 0; i < clientList->size; i++) {
		n = write(clientList->clients[i]->sockfd, message, strlen(message));
		if (n < 0)
			printf("ERROR writing to socket");
	}
}

void printClient(ClientInfo* clientInfo) {
	printf("User %s", clientInfo->userName);
}

void printClients(ClientList* clientList) {
	int i;
	for (i = 0; i < clientList->size; i++) {
		printClient(clientList->clients[i]);
	}
}

void addClient(ClientList* clientList, ClientInfo* clientInfo) {
	if (clientList->size < MAX_CLIENTS) {
		clientList->clients[clientList->size] = clientInfo;
		clientList->size++;
	}
}

void* startClientThread(void* scktInfo) {
	SocketInfo* socketInfo = (SocketInfo*)scktInfo;
	int n;
	int newsockfd = socketInfo->newsockfd;
	char* buffer = socketInfo->buffer;
	ClientList* clientList = socketInfo->clientList;

	while(1) {
		bzero(buffer, 256);

		/* read from the socket */
		n = read(newsockfd, buffer, 256);
		if (n < 0)
			printf("ERROR reading from socket");

		int finalMessageLen = strlen(socketInfo->currentClient->userName) + strlen(buffer) + 4;
		char* finalMessage = malloc(finalMessageLen);
		strcpy(finalMessage, socketInfo->currentClient->userName);
		strcat(finalMessage, " => ");
		strcat(finalMessage, buffer);
		printf("%s\n", finalMessage);

		broadCastMessage(finalMessage, clientList);
	}

	return 0;
}

int main(int argc, char** argv) {
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	ClientList* clientList = malloc(sizeof(ClientList));

	clientList->clients = calloc(MAX_CLIENTS, sizeof(ClientInfo*));
	clientList->size = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

	listen(sockfd, 5);

	clilen = sizeof(struct sockaddr_in);

	while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))) {
		pthread_t child;
		SocketInfo* si = (SocketInfo*)malloc(sizeof(SocketInfo));
		si->buffer = buffer;
		si->newsockfd = newsockfd;		
		ClientInfo* ci = (ClientInfo*)malloc(sizeof(ClientInfo));
		ci->sockfd = newsockfd;
		ci->address = cli_addr;
		sprintf(ci->userName, "anonymous_%d", cli_addr.sin_port);
		addClient(clientList, ci);
		si->clientList = clientList;
		si->currentClient = ci;
		printf("Usuario %s se conectou: \n", ci->userName);
		pthread_create(&child, NULL, startClientThread, si);
	}

	close(newsockfd);
	close(sockfd);
	return 0;
}