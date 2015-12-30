/**
	Author: vipxxq
	Figure 4.8 A linked list implementation.
*/

#ifndef LLIST_H
#define LLIST_H

#include "List.h"
#include "Link.h"
#include <cassert>
#include <iostream>


template <typename E>
class LList: public List<E> {
private:
	Link<E>* head;		// Pointer to list header
	Link<E>* tail;		// Pointer to last element
	Link<E>* curr;		// Access to current element
	int cnt;			// Size of list
	
	void init() {
		curr = tail = head = new Link<E>;
		cnt = 0;
	}
	
	void removeall() {
		while (head != NULL) {
			curr = head;
			head = head->next;
			delete curr;
		}
	}
	
public:
	LList(int size = defaultSize) { init(); }
	~LList() { removeall(); }
	//void print() const;
	void clear() { removeall(); init(); }
	
	// Insert an element at the current location.
	// item: the element to be inserted
	void insert(const E& item) {
		curr->next = new Link<E> (item, curr->next);
		if (tail == NULL) tail = curr->next;
		cnt ++;
	}
	
	// Append an element at the end of the list
	// item: The element to be appended
	void append(const E& item) {
		tail = tail->next = new Link<E> (item, NULL);
		cnt ++;
	}
	
	// Remove and return the current element.
	// Return: the element that removed
	E remove() {
		assert(curr->next != NULL);
		E it = curr->next->element;
		Link<E>* ltemp = curr->next;
		if (tail == curr->next) tail = curr;
		curr->next = curr->next->next;
		delete ltemp;
		cnt --;
		return it;
	}
	
	// Set the current position to the start of the list
	void moveToStart() { curr = head; }
	
	// Set the current position to the end of the list
	void moveToEnd() { curr = tail; }
	
	// Move the current position one step left. No change if already at beginning.
	void prev() {
		if (curr == head) return;
		Link<E>* temp = head;
		while (temp->next != curr) temp = temp->next;
		curr = temp;
	}
	
	// Move the current position one step right. No change if already at end.
	void next() {
		if (curr != tail) curr = curr->next;
	}
	
	// Return: the number of elements in the list.
	int length() const { return cnt; }
	
	// Return: The position of the current element.
	int currPos() const {
		Link<E>* temp = head;
		int i;
		for (i = 0; curr != temp; i++)
			temp = temp->next;
		return i;
	}
	
	// Set current position.
	// pos: the position to make current.
	void moveToPos(int pos) {
		assert((pos >= 0) && (pos <= cnt));
		curr = head;
		for (int i = 0; i < pos; i++) curr = curr->next;
	}
	
	// Return: The current element.
	const E& getValue() const {
		assert(curr->next != NULL);
		return curr->next->element;
	}

};

template <typename E> 
std::ostream& operator << (std::ostream & os, LList<E> &L)
{
	int pos = L.currPos();
	for (L.moveToStart(); L.currPos() < L.length(); L.next())
		os << L.getValue() << " ";
	L.moveToPos(pos);
	return os;
}

#endif