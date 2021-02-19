#ifndef  RENDERERH
#define RENDERERH

#include "Globals.h"

using namespace std;

extern CHAR_INFO *buffer;


void initializeRenderer();
void render();
void printHelperText(string text, int x, int y, int style);

#endif