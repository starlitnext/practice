### Dynamic connectivity
- Union command: connect two objects
- Find/connected query: is there a path connecting the two objects

### Modeling the objects
When programming, convenient to name objects 0 to N-1.
- Use integers as array index.

### Modeling the connections
- Connected components. Maximal set of bojects that are mutually connected.

### Union-find data type (ADT)
Goal. Design efficient data structure for union-find.
- Number of objects N can be huge.
- Number of operations M can be huge.
- Find queries and union commands may be intermixed.

```C++
class UF {
	UF(int N);	// initialize union-find data structure with N objects(0 to N-1)
	
	void union(int p, int q);	// add connection between p and q
	
	bool connected(int p, int q);	// are p and q in the same component?
	
	int find(int p);				// component identifier for p (0 to N-1)
	
	int count();					// number of components
	
};
```

### Quick-find (QuickFindUF.cpp)
> * Datastructure.
	- Integer array id[] of size N.
	- Interpretation: p and q are connected iff they have the same id.
> * Find. Check if p and q have the same id. O(1)
> * Union. To merge components containing p and q, change all entries whose id equals id[p] to id[q]. O(N), Union is too expensive.

### Quick-union (QuickUnionUF.cpp)
> * Datastructure.
	- Integer array id[] of size N.
	- Interpretation: id[i] is parent of i.
	- Root of i is id[id[id[...id[i]...]]].
> * Find. check if p and q have the same root.
> * Union. To merge components containing p and q, set the id of p's root to the id of q's root.

### Improvement
1. Weighted quick-union. (WeightedQuickUnion.cpp)
	- Modify quick-union to avoid tall trees.
	- Keep track of size of each tree (number of objects).
	- Balance by linking root of smaller tree to root of larger tree.
2. Path compression. (Keeps the tree almost completely flat.)
	- Quick union with path compression. Just after computing the root of p, set the id of each examined node to point to that root.
	- Two-pass implementation: add second loop to root() to set the id[] of each examined node to the root.
	- Simple one-pass variant: Make every node in path point to its grandparent (thereby halving path length).
	
### Sumarry
| 		Algorithm		 | 			worst-case time 		|
| 		:-------: 		 | 			:-----------:   		|
| 		quick-find		 |				MN					|
| 		quick-union		 |				MN					|
| 		weighted QU		 |				N + M log N			|
| QU+path compression	 |				N + M log N			|
| weighted QU+path compression|			N + M lg*N			|
