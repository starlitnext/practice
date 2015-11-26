#ifndef LSTACK_H
#define LSTACK_H

#include "Stack.h"
#include "Link.h"
#include <cassert>

// Linked stack implementation
template <typename E>
class LStack: public Stack<E> {
private:
	Link<E>* topPtr;
	int size;
	
public:
	LStack(int sz = defaultSize) { topPtr = NULL; size = 0; }
	
	~LStack() { clear(); }
	
	void clear() {
		while (topPtr != NULL) {
			Link<E>* temp = topPtr;
			topPtr = topPtr->next;
			delete temp;
		}
		size = 0;
	}
	
	void push(const E& it) {
		topPtr = new Link<E>(it, topPtr);
		size ++;
	}
	
	E pop() {
		assert(topPtr != NULL);
		E it = topPtr->element;
		Link<E>* ltemp = topPtr->next;
		delete topPtr;
		topPtr = ltemp;
		size --;
		return it;
	}
	
	const E& top() const {
		assert(topPtr != NULL);
		return topPtr->element;
	}
	
	int length() const { return size; }
};

#endif