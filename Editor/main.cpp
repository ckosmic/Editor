// The Editor by Christian Kosman
// for CS M20 at Moorpark College

#include "Renderer.h"
#include "InputHandler.h"
#include "FileIO.h"

using namespace std;

void main() {
	initializeRenderer();
	string initialFileName = "";
	cout << "Open File (enter file path): " << endl << "> ";
	cin >> initialFileName;
	loadFile(initialFileName);
	revalidateCharacters();
	render();
	while (true) {
		handleInput();
		render();
	}
	system("pause");
}