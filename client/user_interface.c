#include "user_interface.h"

UIStructure* createUI(void) {
	UIStructure* uiStructure = (UIStructure*)malloc(sizeof(UIStructure));
	uiStructure->window = initscr();
	uiStructure->screen = initCDKScreen(uiStructure->window);
	initCDKColor();

	uiStructure->swindow = newCDKSwindow (uiStructure->screen, 0, 0, 18, 80, NULL, 100, TRUE, FALSE);
	uiStructure->mentry = newCDKMentry(uiStructure->screen, 0, 19, NULL, NULL, A_BOLD, ' ', vMIXED, 80, 3, 3, 0, TRUE, FALSE);
	refreshCDKScreen(uiStructure->screen);
	return uiStructure;
}

void destroyUI(UIStructure* uiStucture) {
	destroyCDKSwindow(uiStucture->swindow);
	destroyCDKMentry(uiStucture->mentry);
	destroyCDKScreen(uiStucture->screen);
	endCDK();
}

void printOnChat(UIStructure* uiStructure, char* msg) {
	addCDKSwindow(uiStructure->swindow, msg, BOTTOM);
}

char* getUserInput(UIStructure* uiStructure) {
	char* msg;
	char* finalMessage;
	msg = activateCDKMentry(uiStructure->mentry, NULL);
	refreshCDKScreen(uiStructure->screen);
	finalMessage = (char*)malloc(strlen(msg) + 1);
	strcpy(finalMessage, msg);
	strcat(finalMessage, "\0");
	return finalMessage;
}