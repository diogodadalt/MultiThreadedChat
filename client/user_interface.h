#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <cdk/cdk.h>

typedef struct UIStructure {
	WINDOW *window;
	CDKSCREEN *screen;
	CDKSWINDOW *swindow;
	CDKMENTRY *mentry;
} UIStructure;

UIStructure* createUI(void);
void destroyUI(UIStructure* uiStructure);
void printOnChat(UIStructure* uiStructure, char* msg);
char* getUserInput(UIStructure* uiStructure);

#endif // USER_INTERFACE_H