#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include "user_interface.h"

#define PORT 4000

typedef struct SocketInfo {
	char* buffer;
	int sockfd;
	UIStructure* uiStructure;
} SocketInfo;

char* getUserInput(UIStructure* uiStructure) {
	char* msg;
	msg = activateCDKMentry(uiStructure->mentry, NULL);
	refreshCDKScreen(uiStructure->screen);
	return msg;
}

void* startReading(void* args) {
	SocketInfo* si = (SocketInfo*)args;
	char* buffer = si->buffer;
	int sockfd = si->sockfd;
	int n;

	while(1) {
		bzero(buffer, 256);

		/* read from the socket */
		n = read(sockfd, buffer, 256);
		if (n < 0)
			printf("ERROR reading from socket\n");

		//printf("%s\n", buffer);
		printOnChat(si->uiStructure, buffer);
	}
}

void* startWriting(void* args) {
	SocketInfo* si = (SocketInfo*)args;
	int sockfd = si->sockfd;
	int n;

	while(1) {
		char* userInput = getUserInput(si->uiStructure);

		/* write in the socket */
		n = write(sockfd, userInput, strlen(userInput));
		if (n < 0)
			printf("ERROR writing to socket\n");

		cleanCDKMentry(si->uiStructure->mentry);
	}
}

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	pthread_t rt, wt;
	UIStructure* uiStructure = NULL;

	if (argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		exit(0);
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("ERROR opening socket\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		printf("ERROR connecting\n");
		exit(0);
	}

	SocketInfo* siReading = (SocketInfo*)malloc(sizeof(SocketInfo));
	siReading->buffer = buffer;
	siReading->sockfd = sockfd;
	SocketInfo* siWriting = (SocketInfo*)malloc(sizeof(SocketInfo));
	siWriting->buffer = buffer;
	siWriting->sockfd = sockfd;

	uiStructure = createUI();
	siReading->uiStructure = uiStructure;
	siWriting->uiStructure = uiStructure;

	pthread_create(&rt, NULL, startReading, siReading);
	pthread_create(&wt, NULL, startWriting, siWriting);

	pthread_join(rt, NULL);
	pthread_join(wt, NULL);

	destroyUI(uiStructure);

	close(sockfd);
	return 0;
}
