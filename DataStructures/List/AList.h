#ifndef ALIST_H
#define ALIST_H

#include "List.h"
#include <cassert>
#include <iostream>

// extern const int defaultSize;

// Array-based list implementation
template <typename E>
class AList: public List<E> {
private:
	int maxSize;			// Maximum size of the list
	int listSize;			// Number of list items now
	int curr;				// Position of current element
	E* listArray;			// Array holding list elements
	
public:
	AList(int size = defaultSize) {
		maxSize = size;
		listSize = curr = 0;
		listArray = new E[maxSize];
	}
	
	~AList() { delete [] listArray; } // Destructor
	
	void clear() {
		delete [] listArray;
		listSize = curr = 0;
		listArray = new E[maxSize];
	}
	
	// Insert an element at the current location.
	// item: the element to be inserted
	void insert(const E& item) {
		assert(listSize < maxSize);
		for (int i = listSize; i > curr; i--)
			listArray[i] = listArray[i - 1];
		listArray[curr] = item;
		listSize ++;
	}
	
	// Append an element at the end of the list
	// item: The element to be appended
	void append(const E& item) {
		assert(listSize < maxSize);
		listArray[listSize ++] = item;
	}
	
	// Remove and return the current element.
	// Return: the element that removed
	E remove() {
		assert((curr >= 0) && (curr < listSize));
		E it = listArray[curr];
		for (int i = curr; i < listSize - 1; i++)
			listArray[i] = listArray[i+1];
		listSize --;
		return it;
	}
	
	// Set the current position to the start of the list
	void moveToStart() { curr = 0; }
	
	// Set the current position to the end of the list
	void moveToEnd() { curr = listSize; }
	
	// Move the current position one step left. No change if already at beginning.
	void prev() { if (curr != 0) curr --; }
	
	// Move the current position one step right. No change if already at end.
	void next() { if (curr < listSize) curr ++; }
	
	// Return: the number of elements in the list.
	int length() const { return listSize; }
	
	// Return: The position of the current element.
	int currPos() const { return curr; }
	
	// Set current position.
	// pos: the position to make current.
	void moveToPos(int pos) {
		assert((pos >= 0) && (pos <= listSize));
		curr = pos;
	}
	
	// Return: The current element.
	const E& getValue() const {
		assert((curr >= 0) && (curr < listSize));
		return listArray[curr];
	}
	
	
};

template <typename E> 
std::ostream& operator << (std::ostream & os, AList<E> &L)
{
	int pos = L.currPos();
	for (L.moveToStart(); L.currPos() < L.length(); L.next())
		os << L.getValue() << " ";
	L.moveToPos(pos);
	return os;
}

#endif