#ifndef LIST_H
#define LIST_H

const int defaultSize = 100;

template <typename E> class List {			// List ADT
private:
	void operator = (const List&) { }	// Protect assignment
	List(const List&) { }				// Protect copy constructor	
				
public:
	List() { }							// Default constructor
	virtual ~List() { }					// Base destructor
	
	// Clear contents from the list, to make it empty.
	virtual void clear() = 0;			// pure virtual function
	
	// Insert an element at the current location.
	// item: the element to be inserted
	virtual void insert(const E& item) = 0;
	
	// Append an element at the end of the list
	// item: The element to be appended
	virtual void append(const E& item) = 0;
	
	// Remove and return the current element.
	// Return: the element that removed
	virtual E remove() = 0;
	
	// Set the current position to the start of the list
	virtual void moveToStart() = 0;
	
	// Set the current position to the end of the list
	virtual void moveToEnd() = 0;
	
	// Move the current position one step left. No change if already at beginning.
	virtual void prev() = 0;
	
	// Move the current position one step right. No change if already at end.
	virtual void next() = 0;
	
	// Return: the number of elements in the list.
	virtual int length() const = 0;
	
	// Return: The position of the current element.
	virtual int currPos() const = 0;
	
	// Set current position.
	// pos: the position to make current.
	virtual void moveToPos(int pos) = 0;
	
	// Return: The current element.
	virtual const E& getValue() const = 0;
	
};

#endif