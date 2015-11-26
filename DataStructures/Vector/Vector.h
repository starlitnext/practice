#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

template <typename E>
class Vector {
public:

	explicit Vector( int initSize = 0 )
		: m_size(initSize), m_capacity(initSize + SPARE_CAPACITY)
	{ objects = new E[m_capacity]; }
	
	explicit Vector( int initSize, const E& initVal )
		: m_size(initSize), m_capacity(initSize + SPARE_CAPACITY)
	{ 
		objects = new E[m_capacity];
		for (int i = 0; i < initSize; i++)
			objects[i] = initVal;
	}
	
	Vector( const Vector& rhs ) : objects(NULL)
	{ operator=(rhs); }
	
	~Vector() { delete [] objects; }
	
	const Vector& operator= ( const Vector& rhs ) {
		if (this != &rhs) {
			delete [] objects;
			m_size = rhs.size();
			m_capacity = rhs.capacity();
			
			objects = new E[m_capacity];
			for (int i = 0; i < m_size; i++)
				objects[i] = rhs[i];
		}
		return *this;
	}
	
	E& operator[] ( int index ) { 
		assert((index >= 0) && (index < m_size));
		return objects[index]; 
	}
	
	const E& operator[] ( int index ) const {
		assert((index >= 0) && (index < m_size));
		return objects[index]; 
	}
	
	int size() const { return m_size; }
	
	int capacity() const { return m_capacity; }
	
	bool empty() const { return size() == 0; }
	
	void resize( int newSize ) {
		if (newSize > m_capacity)
			reserve(newSize * 2 + 1);
		m_size = newSize;
	}
	
	void clear() { m_size = 0; }
	
	void push_back( const E& item ) { 
		if (m_size == m_capacity)
			reserve(2 * m_capacity + 1);
		objects[ m_size ++ ] = item;
	}
	
	void pop_back() { m_size --; } 
	
	const E& back() const { return objects[m_size - 1]; }

	typedef E* iterator;
	typedef const E* const_iterator;
	
	iterator begin() { return &objects[0]; }
	const_iterator begin() const { return &objects[0]; }
	
	iterator end() { return &objects[m_size]; }
	const_iterator end() const { return &objects[m_size]; }
	
	enum { SPARE_CAPACITY = 10 };
	
private:

	void reserve( int newCapacity ) {
		assert(newCapacity > m_size);
		
		E* oldArray = objects;
		
		objects = new E[newCapacity];
		for (int i = 0; i < m_size; i++)
			objects[i] = oldArray[i];
		
		m_capacity = newCapacity;
		
		delete [] oldArray;
	}
	
	int m_capacity;
	int m_size;
	E* objects;
};

#endif