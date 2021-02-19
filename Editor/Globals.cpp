#include "Globals.h"

LinkedList* workingFile;
FileInfo fileInfo;
int scroll = 0;

void LinkedList::PushNode(Character ch) {
	Node* node = new Node();
	node->data = ch;
	node->next = NULL;
	if (head == NULL) {
		head = node;
		tail = node;
		node = NULL;
	}
	else {
		tail->next = node;
		tail = node;
	}
	length++;
}

void LinkedList::InsertNode(int pos, Character ch) {
	if (pos > 1) {
		Node* previous = new Node();
		Node* current = new Node();
		Node* tmp = new Node();
		current = head;
		for (int i = 1; i < pos; i++) {
			previous = current;
			current = current->next;
		}
		tmp->data = ch;
		previous->next = tmp;
		tmp->next = current;
		length++;
	} else if (pos == 1) {
		Node* newHead = new Node();
		newHead->data = ch;
		newHead->next = head;
		head = newHead;
		length++;
	}
}

void LinkedList::RemoveNode(int pos) {
	if (pos > 1) {
		Node* previous = new Node();
		Node* current = new Node();
		Node* next = new Node();
		current = head;
		for (int i = 0; i < pos - 1; i++) {
			previous = current;
			current = current->next;
			next = current->next;
		}
		previous->next = next;
		delete(current);
		length--;
	} else if(pos == 1) {
		Node* tmp = new Node();
		tmp = head;
		head = head->next;
		delete(tmp);
		length--;
	}
}

int LinkedList::Length() {
	return length;
}
Node* LinkedList::GetHead() {
	return head;
}
Node* LinkedList::GetTail() {
	return tail;
}

Node* getSelectedNode() {
	Node* current = workingFile->GetHead();
	while (current->data.selected == false) {
		current = current->next;
	}
	return current;
}

Node* getNodeByRowColumn(int row, int column) {
	Node* current = workingFile->GetHead();
	while (current->data.row != row || current->data.column != column) {
		if(current->next != NULL)
			current = current->next;
		if (current->data.row == row && current->data.ch == 3) break;
	}
	return current;
}

int getIndexOfNode(Node* node) {
	Node* current = workingFile->GetHead();
	int i = 0;
	while (current != node) {
		current = current->next;
		i++;
	}
	return i;
}

void revalidateCharacters() {
	Node* current = workingFile->GetHead();
	int row = 0;
	int column = 0;
	fileInfo.totalLines = 0;
	while (current != NULL) {
		current->data.column = column;
		current->data.row = row;
		if (current->data.ch == 3) {
			row++;
			column = 0;
			fileInfo.totalLines++;
		} else {
			column++;
		}

		current = current->next;
	}
	fileInfo.totalLines--;
}

