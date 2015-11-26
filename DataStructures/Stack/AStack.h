#ifndef ASTACK_H
#define ASTACK_H

#include "Stack.h"
#include <cassert>

// Array-based stack implementation
template <typename E> 
class AStack: public Stack<E> {
private:
	int maxSize;	
	int topIndex;
	E * listArray;
	
public:
	AStack(int size = defaultSize) {
		maxSize = size; 
		topIndex = 0;
		listArray = new E[size];
	}
	
	~AStack() { delete [] listArray; }
	
	void clear() { topIndex = 0; }
	
	void push(const E& it) {
		assert(topIndex != maxSize);
		listArray[topIndex ++] = it;
	}
	
	E pop() {
		assert(topIndex != 0);
		return listArray[-- topIndex];
	}
	
	const E& top() const {
		assert(topIndex != 0);
		return listArray[topIndex-1];
	}
	
	int length() const { return topIndex; }
	
};

#endif