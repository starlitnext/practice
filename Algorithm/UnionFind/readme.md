###Dynamic connectivity
- Union command: connect two objects
- Find/connected query: is there a path connecting the two objects

###Modeling the objects
When programming, convenient to name objects 0 to N-1.
- Use integers as array index.

###Modeling the connections
- Connected components. Maximal set of bojects that are mutually connected.

###Union-find data type (ADT)
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