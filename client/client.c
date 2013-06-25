#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include "../common/socket_wrapper.h"
#include "user_interface.h"

#define PORT 4000
#define BUFFER_SIZE 256

int isQuitCommand(char* message) {
	if (message != NULL && strlen(message) > 0 && message[0] == '#') {
		if (strcmp(message, "#QUIT") == 0 || strcmp(message, "#q") == 0)
			return 1;
		else
			return 0;
	} else
		return 0;
}

void* startReading(void* args) {
	ConnectionInfo* ci = (ConnectionInfo*)args;
	char buffer[BUFFER_SIZE];
	int sockfd = ci->sockfd;
	UIStructure* uiStructure = (UIStructure*)ci->additionalInfo;
	int n;

	while(1) {
		bzero(buffer, BUFFER_SIZE);

		/* read from the socket */
		n = read(sockfd, buffer, BUFFER_SIZE);
		if (n < 0)
			printf("ERROR reading from socket\n");

		printOnChat(uiStructure, buffer);
	}
}

void* startWriting(void* args) {
	ConnectionInfo* ci = (ConnectionInfo*)args;
	int sockfd = ci->sockfd;
	UIStructure* uiStructure = (UIStructure*)ci->additionalInfo;
	int n;

	while(1) {
		char* userInput = getUserInput(uiStructure);

		/* write in the socket */
		n = write(sockfd, userInput, strlen(userInput));
		if (n < 0)
			printf("ERROR writing to socket\n");

		cleanCDKMentry(uiStructure->mentry);

		if (isQuitCommand(userInput)) {
			close(ci->sockfd);
			destroyUI(uiStructure);
			exit(0);
		}
	}
}

void* handleConnection(void* args) {
	ConnectionInfo* connInfo = (ConnectionInfo*)args;
	pthread_t rt, wt;
	UIStructure* uiStructure = NULL;

	uiStructure = createUI();
	connInfo->additionalInfo = uiStructure;

	pthread_create(&rt, NULL, startReading, connInfo);
	pthread_create(&wt, NULL, startWriting, connInfo);

	pthread_join(rt, NULL);
	pthread_join(wt, NULL);

	destroyUI(uiStructure);
	return 0;
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		exit(0);
	}

	connectClientBlocking(argv[1], PORT, handleConnection, NULL);

	return 0;
}
