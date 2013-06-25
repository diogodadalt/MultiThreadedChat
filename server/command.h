#ifndef COMMAND_H
#define COMMAND_H

#include "../server/client_info.h"

int processCommandIfExist(char* message, ClientInfo* ci);
void processCommand(char* cmd, ClientInfo* ci);

#endif