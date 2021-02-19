#ifndef INPUTHANDLERH
#define INPUTHANDLERH

#include "Globals.h"
#include "FileIO.h"

using namespace std;

struct key {
	short newState;
	short oldState;
	bool pressed;
	bool released;
	bool held;
};

extern key keys[256];

void handleInput();
void refactorSelected();
void addCharacter(char ch);

#endif