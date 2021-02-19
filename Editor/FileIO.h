#ifndef FILEIOH
#define FILEIOH

#include "Globals.h"

using namespace std;

LinkedList* loadFile(string fileName);
void saveFile();
void addCharacterToWorkingFile(char character, int column, int row);

#endif