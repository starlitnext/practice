#ifndef BST_H
#define BST_H

#include <iostream>

#include "../Dictionary/Dictionary.h"
#include "BSTNode.h"

using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <typename Key, typename E>
class BST : public Dictionary<Key, E> {
private:
	BSTNode<Key, E>* root;				// Root of the BST
	int nodecount;						// Number of nodes in the BST
	
	// Private helper functions.
	void clearhelp(BSTNode<Key, E>*);
	BSTNode<Key, E>* inserthelp(BSTNode<Key, E>*, const Key&, const E&);
	BSTNode<Key, E>* deletemin(BSTNode<Key, E>*);
	BSTNode<Key, E>* removehelp(BSTNode<Key, E>*, const Key&);
	E findhelp(BSTNode<Key, E>*, const Key&) const;
	void printhelp(BSTNode<Key, E>*, int) const;
	
public:
	BST() { root = NULL; nodecount = 0; }
	~BST() { clearhelp(root); }
	
	// Reinitialize tree
	void clear() {
		clearhelp(root);
		root = NULL;
		nodecount = 0;
	}
	
	// Insert a record into the tree.
	// k Key value of the record.
	// e The record to insert.
	void insert(const Key& k, const E& e) {
		root = inserthelp(root, k, e);
		nodecount ++;
	}
	
	// Remove a record from the tree.
	// k Key value of record to remove.
	// Return: The record removed, or NULL if there is none.
	E remove(const Key& k) {
		E temp = findhelp(root, k);
		if (temp != NULL) {
			root = removehelp(root, k);
			nodecount --;
		}
		return temp;
	}
	
	// Remove and return the root node from the dictionary.
	// Return: The record removed, null if tree is empty.
	E removeAny() {		// Delete min value
		if (root != NULL) {
			E temp = root->element();
			root = removehelp(root, root->key());
			nodecount --;
			return temp;
		}
		else return NULL;
	}
	
	// Return record with key value k, NULL if none exist.
	// k: the key value to find.
	// Return: A record matching "k" (NULL if non exists).
	// If multiple records match, return an arbitrary one.
	E find(const Key& k) const {
		return findhelp(root, k);
	}
	
	// Return the number of records in the dictionary
	int size() { return nodecount; }
	
	void print() const {
		if (root == NULL) cout << "The BST is empty." << endl;
		else printhelp(root, 0);
	}
};

template <typename Key, typename E>
E BST<Key, E>::findhelp(BSTNode<Key, E>* root, const Key& k) const {
	if (root == NULL) return NULL;
	if (k < root->key())
		return findhelp(root->left(), k);
	else if (k > root->key())
		return findhelp(root->right(), k);
	else return root->element();
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(BSTNode<Key, E>* root, const Key& k, const E& it) {
	if (root == NULL)
		return new BSTNode<Key, E>(k, it, NULL, NULL);
	if (k < root->key())
		root->setLeft(inserthelp(root->left(), k, it));
	else root->setRight(inserthelp(root->right(), k, it));
	return root;
}

#endif // BST_H