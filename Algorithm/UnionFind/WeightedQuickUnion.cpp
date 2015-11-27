#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class WeightedQuickUnion {
public:
	
	WeightedQuickUnion(int n) : N(n) {
		id = new int[N];
		sz = new int[N];
		for (int i = 0; i < N; i++)
		{
			id[i] = i;
			sz[i] = 1;
		}
	}
	
	void Union(int p, int q) {
		int i = root(p);
		int j = root(q);
		if (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
		else { id[j] = i; sz[i] += sz[j]; }
	}
	
	bool Connected(int p, int q) {
		return root(p) == root(q);
	}
	

private:

	int root(int i) {
		while (i != id[i]) { id[i] = id[id[i]]; i = id[i]; }
		return i;
	}

	int *id;
	int *sz;
	int N;
};

int main() {
	ifstream fin("tinyUF.txt");
	int n;
	fin >> n;
	WeightedQuickUnion uf(n);
	int p, q;
	clock_t t;
	t = clock();
	while (fin >> p >> q) {
		cout << p << " " << q << endl;
		if (!uf.Connected(p, q)) {
			uf.Union(p, q);
		}
		else  {
			cout << "is already connected." << endl;
		}
	}
	t = clock() - t;
	cout << "It takes " << t << " clicks (" << ((float)t)/CLOCKS_PER_SEC << " seconds.)" << endl;
	return 0;
}