#ifndef  SALDICT_H
#define SALDICT_H

#include "SAList.h"
#include "KVpair.h"

// Dictionary implemented with a sorted array-based list
template <typename Key, typename E>
class SALdict : public Dictionary<Key, E> {
private:
	SAList<Key, E>* list;
	
public:
	SALdict(int size = defaultSize) { list = new SAList<Key, E>(size); }
	~SALdict() { delete list; }
	
	void clear() { list->clear(); }
	
	void insert(const Key& k, const E& e) {
		KVpair<Key, E> temp(k, e);
		list->insert(temp);
	}
	
	E remove(const Key& k) {
		E temp = find(k);
		if (temp != NULL) list->remove();
		return temp;
	}
	
	// Remove and return an arbitrary record from dictionary.
	// Return: The record removed, or NULL if non exists.
	E removeAny() {
		assert(size() != 0);
		list->moveToEnd();
		list->prev();
		KVpair<Key, E> e = list->remove();
		return e.value();
	}
	
	// Return: A record matching "k" (NULL if non exists).
	// If multiple records match, return an arbitrary one.
	// k: The key of the record to find.
	E find(const Key& k) const {
		int l = -1;
		int r = list->length();
		while (l+1 != r) {
			int i = (l + r) / 2;
			list->moveToPos(i);
			KVpair<Key, E> temp = list->getValue();
			if (k < temp.key()) r = i;
			else if (k == temp.key()) return temp.value();
			if (k > temp.key()) l = i;
		}
		return NULL;
	}
	
	// Return the number of records in the dictionary.
	int size() {
		return list->length();
	}
};

#endif // SALDICT_H