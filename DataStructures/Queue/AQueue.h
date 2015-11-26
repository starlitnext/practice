#ifndef AQUEUE_H
#define AQUEUE_H

#include "Queue.h"
#include <cassert>

// Array-based queue implementation
template <typename E>
class AQueue: public Queue<E> {
private:
	int maxSize;
	int front;
	int rear;
	E* listArray;
	
public:
	AQueue(int size = defaultSize) {
		maxSize = size + 1;
		rear = 0; front = 1;
		listArray = new E[maxSize];
	}
	
	~AQueue() { delete [] listArray; }
	
	void clear() { rear = 0; front = 1; }
	
	void enqueue(const E& it) {
		assert(((rear+2) % maxSize) != front);
		rear = (rear+1) % maxSize;
		listArray[rear] = it;
	}
	
	E dequeue() {
		assert(length() != 0);
		E it = listArray[front];
		front = (front+1) % maxSize;
		return it;
	}
	
	const E& frontValue() const {
		assert(length() != 0);
		return listArray[front];
	}
	
	int length() const {
		return ((rear + maxSize) - front + 1) % maxSize;
	}
	
};

#endif