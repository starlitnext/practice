#ifndef SALIST_H
#define SALIST_H

#include "../List/AList.h"
#include "KVpair.h"

// Sorted array-based list
// Inherit from AList as a protected base class
template <typename Key, typename E>
class SAList : protected AList<KVpair<Key, E>> {
public:
	SAList(int size = defaultSize) : 
		AList<KVpair<Key, E>>(size) { }
	~SAList() { }
	
	// Redefine insert function to keep values sorted
	void insert(KVpair<Key, E>& it) {
		KVpair<Key, E> curr;
		for (moveToStart(); currPos() < length(); next()) {
			curr = getValue();
			if (curr.key() > it.key())
				break;
		}
		AList<KVpair<Key, E>>::insert(it);
	}
	
	// With the exception of append, all remaining mothods are
	// exposed from AList. Append is not available to SAList
	// class users since it has not been explicitly exposed.
	AList<KVpair<Key, E>>::clear;
	AList<KVpair<Key, E>>::remove;
	AList<KVpair<Key, E>>::moveToStart;
	AList<KVpair<Key, E>>::moveToEnd;
	AList<KVpair<Key, E>>::prev;
	AList<KVpair<Key, E>>::next;
	AList<KVpair<Key, E>>::length;
	AList<KVpair<Key, E>>::currPos;
	AList<KVpair<Key, E>>::moveToPos;
	AList<KVpair<Key, E>>::getValue;
};

#endif // SALIST_H