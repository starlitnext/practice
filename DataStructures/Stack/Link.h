#ifndef LINK_H
#define LINK_H

// Singly linked list node with freelist support
template <typename E> class Link {
private: 
	static Link<E>* freelist;	// Reference to freelist head
public:
	E element;
	Link* next;
	
	Link(const E& elemval, Link* nextval = NULL)
	{ element = elemval; next = nextval; }
	Link(Link* nextval = NULL) { next = nextval; }
	
	void* operator new(size_t) {
		if (freelist == NULL) return ::new Link;
		Link<E>* temp = freelist;	// Can take from freelist
		freelist = freelist->next;
		return temp;		
	}
	
	void operator delete(void* ptr) {
		((Link<E>*) ptr)->next = freelist;	// Put on freelist
		freelist = (Link<E>*) ptr;
	}
	
};

// The freelist head pointer is actually created here
template <typename E>
Link<E>* Link<E>::freelist = NULL;

/*
template <typename E> class Link {
public:
	E element;
	Link *next;
	Link(const E& elemval, Link* nextval = NULL)
	{ element = elemval; next = nextval; }
	Link(Link* nextval = NULL) { next = nextval; }
};
*/

#endif