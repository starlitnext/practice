#ifndef UALDICT_H
#define UALDICT_H

#include "KVpair.h"
#include "Dictionary.h"
#include "../List/Alist.h"

// Dictionary implemented with an unsorted array-based list
template <typename Key, typename E>
class UALdict : public Dictionary<Key, E> {
private:
	AList<KVpair<Key, E>>* list;
public:
	UALdict(int size = defaultSize) { list = new AList<KVpair<Key, E>>(size); }
	~UALdict() { delete list; }
	void clear() { list->clear(); }
	
	// Insert a record
	// k: The key for record being inserted.
	// e: The record being inserted.
	void insert(const Key& k, const E& e) {
		KVpair<Key, E> temp(k, e);
		list->append(temp);
	}
	
	// Remove and return a record.
	// k: The key of the record to be removed.
	// Return: A maching record. If multiple records match
	// "k", remove any arbitrary one. Return NULL if no record
	// with key "k" exists.
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
		for (list->moveToStart(); list->currPos() < list->length(); list->next()) {
			KVpair<Key, E> temp = list->getValue();
			if (k == temp.key())
				return temp.value();
		}
		return NULL;
	}
	
	// Return the number of records in the dictionary.
	int size() {
		return list->length();
	}
	

};

#endif // UALDICT_H