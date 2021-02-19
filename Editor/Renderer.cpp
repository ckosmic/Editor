#include "Renderer.h"
#include "FileIO.h"

CHAR_INFO *buffer;

HANDLE outputHandle;
HANDLE inputHandle;
SMALL_RECT writeArea;


// Sets up the console to render as a buffer (it's a lot easier like this than a cout system)
void initializeRenderer() {
	// Initialize the screen buffer
	buffer = new CHAR_INFO[BUFFER_SIZE];
	memset(buffer, 0, sizeof(CHAR_INFO) * BUFFER_SIZE);

	// Grab the console window handle as well as its input/output handles
	HWND consoleWindow = GetConsoleWindow();
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	// Hide the built-in cursor, cuz I made my own
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(outputHandle, &cci);
	cci.bVisible = false;
	SetConsoleCursorInfo(outputHandle, &cci);

	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	writeArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	// Set the window's pixel size
	SetConsoleWindowInfo(outputHandle, TRUE, &writeArea);
	// Set the size of the console (rows and columns) to the same as the buffer
	SetConsoleScreenBufferSize(outputHandle, bufferSize);

	DWORD consoleMode;
	GetConsoleMode(inputHandle, &consoleMode);
	SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | (consoleMode & ~ENABLE_QUICK_EDIT_MODE));

	DWORD consoleStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, consoleStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	ShowWindow(consoleWindow, SW_SHOW);

	// Center the window on the screen
	RECT rectWindow;
	GetWindowRect(consoleWindow, &rectWindow);
	int posX, posY;
	posX = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posY = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
	MoveWindow(consoleWindow, posX, posY, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, TRUE);
}

void render() {
	// Set scroll position
	Node* current = workingFile->GetHead();
	int line = 0;
	while (line < scroll) {
		if (current->data.ch == 3) line++;
		current = current->next;
	}

	// Default character style
	int defaultStyle = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	// Style when a character is selected
	int selectedStyle = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
	// Style for tips bar
	int tipsStyle = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE;

	// Clear buffer for redraw
	memset(buffer, 0, sizeof(CHAR_INFO) * BUFFER_SIZE);

	line = 0;
	Node* selectedNode = getSelectedNode();
	int lineScroll = 0;
	while (selectedNode->data.column - lineScroll > SCREEN_WIDTH - 1) {
		lineScroll++;
	}
	while (current != NULL && line < WORK_HEIGHT) {
		int column = current->data.column - lineScroll;
		bool offScreen = column > SCREEN_WIDTH - 1;
		if (column < 0) {
			offScreen = true;
			column = 0;
		}
		int index = SCREEN_WIDTH * (current->data.row - scroll) + column;
		// If unknown character, don't render it or else it'll show a question mark
		if(current->data.ch != 3 && current->data.ch != '\t' && !offScreen)
			buffer[index].Char.AsciiChar = current->data.ch;

		if(current->data.ch == 3) line++;

		if(current->data.selected)
			buffer[index].Attributes = selectedStyle;
		else
			buffer[index].Attributes = defaultStyle;
		current = current->next;
	}

	int start = 20 * SCREEN_WIDTH;
	for (int i = start; i < BUFFER_SIZE; i++) {
		buffer[i].Char.AsciiChar = ' ';
		buffer[i].Attributes = tipsStyle;
	}

	printHelperText("Ctrl + S - Save File", 1, SCREEN_HEIGHT - 4, tipsStyle);
	printHelperText("Arrow Keys - Navigate", 1, SCREEN_HEIGHT - 2, tipsStyle);
	printHelperText("Ctrl + O - Open File", 30, SCREEN_HEIGHT - 4, tipsStyle);
	printHelperText("Escape - Exit Program", 30, SCREEN_HEIGHT - 2, tipsStyle);

	// Set the title of the window.  If unsaved, put an asterisk.  If saved, leave it as normal
	string titleName = "M20 Editor - " + string(fileInfo.filePath);
	if (fileInfo.unsaved)
		titleName += "*";
	wstring wTitleName(titleName.length(), L' ');
	copy(titleName.begin(), titleName.end(), wTitleName.begin());
	SetConsoleTitle(wTitleName.c_str());

	// Render the buffer to the console window
	WriteConsoleOutput(outputHandle, buffer, { SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0 }, &writeArea);
}

// This is just helper text, it has nothing to do with the edited text!
void printHelperText(string text, int x, int y, int style) {
	int start = SCREEN_WIDTH * y + x;
	for (int i = start; i < start + text.length(); i++) {
		buffer[i].Char.AsciiChar = text[i - start];
		buffer[i].Attributes = style;
	}
}