#include "FileIO.h"

LinkedList* loadFile(string fileName) {
	// Initialize the working file linked list
	workingFile = new LinkedList();

	fileInfo.filePath = fileName;

	// Open a file
	ifstream str(fileName, ios::binary|ios::ate);
	// Did the file fail to open?  If so, try asking again.
	if (str.fail()) {
		string fn = "";
		cout << "The file failed to open.  Please try again." << endl;
		cout << "Open File (enter file path): " << endl << "> ";
		cin.ignore();
		cin >> fn;
		cout << fn;
		loadFile(fn);
		return NULL;
	}
	ifstream::pos_type pos = str.tellg();

	// Get the length of the file's characters (bytes)
	int length = pos;

	char* data = new char[length];
	str.seekg(0, ios::beg);
	str.read(data, length);

	str.close();

	int column = 0;
	int row = 0;
	fileInfo.totalLines = 0;
	// Loop through file and get its characters
	for (int i = 0; i < length; i++) {
		// If character is not an end-of-line character, add it to the linked list
		if (data[i] != '\n' && data[i] != '\r') {
			char character = data[i];
			if (data[i] == '\t') {
				// Convert tabs to 4 spaces because tabs are just too weird to deal with
				for(int j = 0; j < 4; j++)
					addCharacterToWorkingFile(' ', column, row);
			} else {
				addCharacterToWorkingFile(character, column, row);
			}

			column++;
		}
		// If character IS an end-of-line character, add char 3 (end of text) character to denote end of line
		if (data[i] == '\n') {
			addCharacterToWorkingFile(3, column, row); // 3 is the end of text character, used for end of line selection

			column = 0;
			row++;
			fileInfo.totalLines++;
		}
	}
	// Add an end of line character to the end of the linked list
	addCharacterToWorkingFile(3, column, row); // 3 is the end of text character, used for end of line selection

	// Set the currently selected character to the first one in the linked list
	workingFile->GetHead()->data.selected = true;

	return workingFile;
}

void saveFile() {
	Node* current = new Node();
	current = workingFile->GetHead();
	string data = "";
	while (current != NULL) {
		char ch = current->data.ch;
		if (ch == 3) ch = '\n';
		data += ch;
		current = current->next;
	}

	ofstream outFile(fileInfo.filePath);
	outFile << data;
	outFile.close();
}

void addCharacterToWorkingFile(char character, int column, int row) {
	Character ch;
	ch.ch = character;
	ch.column = column;
	ch.row = row;
	ch.selected = false;
	workingFile->PushNode(ch);

	string readFile = "";
	for (int i = 0; i < readFile.length(); i++) {
		readFile[i];
	}
}
