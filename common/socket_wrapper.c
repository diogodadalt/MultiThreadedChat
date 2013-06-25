#include "socket_wrapper.h"

void connectServerBlocking(int port, int maxConns, void* (*socketHandler)(void*), void* additionalInfo) {
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

	listen(sockfd, maxConns);

	clilen = sizeof(struct sockaddr_in);

	while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))) {
		ConnectionInfo* connInfo = malloc(sizeof(ConnectionInfo));
		connInfo->sockfd = newsockfd;
		connInfo->address = cli_addr;
		connInfo->additionalInfo = additionalInfo;
		socketHandler(connInfo);
	}

	close(newsockfd);
	close(sockfd);
}

void connectClientBlocking(char* host, int port, void* (*socketHandler)(void*), void* additionalInfo) {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent* server;

	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("ERROR opening socket\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		printf("ERROR connecting\n");
		exit(0);
	}

	ConnectionInfo* connInfo = malloc(sizeof(ConnectionInfo));
	connInfo->sockfd = sockfd;
	connInfo->address = serv_addr;
	connInfo->additionalInfo = additionalInfo;
	socketHandler(connInfo);

	close(sockfd);
}
