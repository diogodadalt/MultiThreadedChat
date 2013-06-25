#include "command.h"

int processCommandIfExist(char* message, ClientInfo* ci) {
	if (message != NULL && strlen(message) > 0 && message[0] == '#') {
		processCommand(message, ci);
		return 1;
	} else
		return 0;
}

void processCommand(char* cmd, ClientInfo* ci) {
	if (strcmp(cmd, "#QUIT") == 0 || strcmp(cmd, "#q") == 0) {
		removeClient(ci->clientList, ci);
	}
}