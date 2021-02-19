#ifndef GLOBALSH
#define GLOBALSH

#include <fstream>
#include <conio.h>
#include <string>
#include <iostream>
#include <Windows.h>

#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 25
#define BUFFER_SIZE SCREEN_HEIGHT*SCREEN_WIDTH
#define WORK_HEIGHT SCREEN_HEIGHT-5

using namespace std;

struct Character {
	char ch;
	int column;
	int row;
	bool selected;
};

struct Node {
	Character data;
	Node* next;
};

struct FileInfo {
	int totalLines;
	bool unsaved;
	string filePath;
};

class LinkedList {
private:
	Node* head;
	Node* tail;
	int length;
public:
	LinkedList() { head = NULL; tail = NULL; length = 0; }
	void PushNode(Character ch);
	void InsertNode(int pos, Character ch);
	void RemoveNode(int pos);

	int Length();
	Node* GetHead();
	Node* GetTail();
};

extern LinkedList* workingFile;
extern FileInfo fileInfo;
extern int selected;
extern int scroll;

Node* getSelectedNode();
Node* getNodeByRowColumn(int row, int column);
int getIndexOfNode(Node* node);
void revalidateCharacters();

#endif


