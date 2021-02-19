#include "InputHandler.h"

key keys[256];
int selected = 0;

void handleInput() {
	// Get pressed key
	char ch = getch();

	if (ch == 77) { // Right arrow
		// Increment selected to point to the next character
		selected++;
		// Assign the selected character's selected property
		refactorSelected();
		// Scroll the editor if the newly selected character is below the work area
		Node* selectedNode = getSelectedNode();
		if (selectedNode->data.row - scroll > WORK_HEIGHT - 1) scroll++;
	} else if (ch == 75) { // Left arrow
		// Decrement selected to point to the previous character
		selected--;
		// Assign the selected character's selected property
		refactorSelected();
		// Scroll the editor if the newly selected character is above the work area
		Node* selectedNode = getSelectedNode();
		if (selectedNode->data.row - scroll < 0) scroll--;
	} else if (ch == 72) { // Up arrow
		Node* selectedNode = getSelectedNode();
		// If the current line is not the first one, go up a line
		if (selectedNode->data.row > 0) {
			// Get the character directly above the currently selected character
			Node* newSelectedNode = getNodeByRowColumn(selectedNode->data.row - 1, selectedNode->data.column);
			// Select the newly found character
			selected = getIndexOfNode(newSelectedNode);
			selectedNode->data.selected = false;
			newSelectedNode->data.selected = true;

			// If cursor is on the top row, scroll up
			if (selectedNode->data.row - scroll == 0) scroll--;
		}
	} else if (ch == 80) { // Down arrow
		Node* selectedNode = getSelectedNode();
		// If the current line is not the first one, go down a line
		if (selectedNode->data.row < fileInfo.totalLines) {
			// Get the character directly below the currently selected character
			Node* newSelectedNode = getNodeByRowColumn(selectedNode->data.row + 1, selectedNode->data.column);
			// Select the newly found character
			selected = getIndexOfNode(newSelectedNode);
			selectedNode->data.selected = false;
			newSelectedNode->data.selected = true;
			
			// If cursor is on the bottom row, scroll down
			if (selectedNode->data.row - scroll == WORK_HEIGHT - 1) scroll++;
		}
	} else if (ch == 13) { // Enter key
		addCharacter(3);
		fileInfo.unsaved = true;
	} else if (ch == 8) { // Backspace
		Node* selectedNode = getSelectedNode();
		int index = getIndexOfNode(selectedNode);
		if (index > 0) {
			workingFile->RemoveNode(index);
			selected--;
			fileInfo.unsaved = true;
		}
	} else if (('s' & 0x1f) == ch) { // Ctrl + S
		saveFile();
		fileInfo.unsaved = false;
	} else if (('o' & 0x1f) == ch) { // Ctrl + O
		system("cls");
		string fileName;
		cout << "Please enter a file to open:\n";
		cin >> fileName;
		loadFile((char*)fileName.c_str());
	} else if (ch == 27) { // Escape
		exit(0);
	} else if (ch > 31 && ch < 127) { // Any other character
		addCharacter(ch);
		fileInfo.unsaved = true;
	}

	// Reformat the editor's linked list if needed
	revalidateCharacters();
}

// Sets the currently selected character
void refactorSelected() {
	if (selected < 0) selected = 0;
	if (selected > workingFile->Length() - 1) selected = workingFile->Length() - 1;
	Node* current = workingFile->GetHead();
	int i = 0;
	while(current != NULL) {
		current->data.selected = i == selected;
		current = current->next;
		i++;
	}
}

// Adds a new character before the selected character
void addCharacter(char ch) {
	Node* selectedNode = getSelectedNode();
	int index = getIndexOfNode(selectedNode) + 1;
	Character chcaracter;
	chcaracter.ch = ch;
	chcaracter.column = selectedNode->data.column;
	chcaracter.row = selectedNode->data.row;
	chcaracter.selected = false;
	workingFile->InsertNode(index, chcaracter);
	selected++;
}