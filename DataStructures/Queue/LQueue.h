#ifndef LQUEUE_H
#define LQUEUE_H

#include "Queue.h"
#include "Link.h"
#include <cassert>

// Linked queue implementation
template <typename E> 
class LQueue: public Queue<E> {
private:
	Link<E>* front;
	Link<E>* rear;
	int size;
	
public:
	LQueue(int sz = defaultSize) {
		front = rear = new Link<E>();
		size = 0;
	}
	
	~LQueue() { clear(); delete front; }
	
	void clear() {
		while (front->next != NULL) {
			rear = front;
			front = front->next;
			delete rear;
		}
		rear = front;
		size = 0;
	}
	
	void enqueue(const E& it) {
		rear->next = new Link<E>(it, NULL);
		rear = rear->next;
		size ++;
	}
	
	E dequeue() {
		assert(size != 0);
		E it = front->next->element;
		Link<E>* ltemp = front->next;
		front->next = ltemp->next;
		if (rear == ltemp) rear = front;
		delete ltemp;
		size --;
		return it;
	}
	
	const E& frontValue() const {
		assert(size != 0);
		return front->next->element;
	}
	
	int length() const { return size; }
	
};

#endif