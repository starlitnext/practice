#ifndef BINNODE_H
#define BINNODE_H

// Binary tree node abstract class
template <typename E>
class BinNode {
public:
	virtual ~BinNode() { }
	
	// Return the node's value
	virtual E& element() = 0;
	
	// Set the node's value
	virtual void setElement(const E& ) = 0;
	
	// Return the node's left child
	virtual BinNode* left() const = 0;
	
	// Set the node's left child
	virtual void setLeft(BinNode*) = 0;
	
	// Return the node's right child
	virtual BinNode* right() const = 0;
	
	// Set the node's right child
	virtual void setRight(BinNode*) = 0;
	
	// Return ture if the node is a leaf, false otherwise
	virtual bool isLeaf() = 0;
};

// Preorder traversal


// Inoder traversal


// Postorder traversal


// count the non-empty nodes of the tree
template <typename E>
int count(BinNode<E>* root) {
	if (root == NULL) return 0;
	return 1 + count(root->left()) + count (root->right());
}

#endif // BINNODE_H