#ifndef BSTNODE_H
#define BSTNODE_H

#include "BinNode.h"

// Simple binary tree node implementation
template <typename Key, typename E>
class BSTNode : public BinNode<E> {
private:
	Key k;						// The node's key_comp
	E it;						// The node's value
	BSTNode* lc;				// Pointer to left child
	BSTNode* rc;				// Pointer to right child
	
public:
	BSTNode() { lc = rc = NULL; }
	BSTNode(Key K, E e, BSTNode* l = NULL, BSTNode* r = NULL)
	{ k = K; it = e; lc = l; rc = r; }
	~BSTNode() { }
	
	// Functions to set and return the value and key
	E& element() { return it; }
	void setElement(const E& e) { it = e; }
	Key& key() { return k; }
	void setKey(const Key& K) { k = K; }
	
	// Functions to set and return children
	inline BSTNode* left() const { return lc; }
	void setLeft(BinNode<E>* b) { lc = (BSTNode*)b; }
	inline BSTNode* right() const { return rc; }
	void setRight(BinNode<E>* b) { rc = (BSTNode*)b; }
	
	// Return true if it is a leaf, false otherwise
	bool isLeaf() { return (lc == NULL) && (rc == NULL); }
};

#endif // BSTNODE_H