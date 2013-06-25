#include "client_info.h"

pthread_mutex_t client_list_mtx = PTHREAD_MUTEX_INITIALIZER;

void addClient(ClientList* clientList, ClientInfo* clientInfo, int maxClients) {
	pthread_mutex_lock(&client_list_mtx);
	if (clientList->size < maxClients) {
		clientList->clients[clientList->size] = clientInfo;
		clientList->size++;
	}
	pthread_mutex_unlock(&client_list_mtx);
}

void removeClient(ClientList* clientList, ClientInfo* clientInfo) {
	int i;
	pthread_mutex_lock(&client_list_mtx);
	for (i = 0; i < clientList->size; i++) {
		if (clientList->clients[i] == NULL)
			break;

		if (strcmp(clientList->clients[i]->userName, clientInfo->userName) == 0) {
			clientList->clients[i] = 0;
			clientList->size--;
			break;
		}
	}
	pthread_mutex_unlock(&client_list_mtx);
}

pthread_mutex_t getClientsMutex() {
	return client_list_mtx;
}

void printClient(ClientInfo* clientInfo) {
	printf("User %s", clientInfo->userName);
}

void printClients(ClientList* clientList) {
	int i;
	pthread_mutex_lock(&client_list_mtx);
	for (i = 0; i < clientList->size; i++) {
		printClient(clientList->clients[i]);
	}
	pthread_mutex_unlock(&client_list_mtx);
}

char* assembleMessage(ClientInfo* clientInfo, char* message) {
	int finalMessageLen = strlen(clientInfo->userName) + strlen(message) + 4;
	char* finalMessage = malloc(finalMessageLen);
	strcpy(finalMessage, clientInfo->userName);
	strcat(finalMessage, " => ");
	strcat(finalMessage, message);
	return finalMessage;
}
