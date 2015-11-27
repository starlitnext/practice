#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class QuickUnionUF {
public:
	QuickUnionUF(int n) : N(n) {
		id = new int[N];
		for (int i = 0; i < N; i++)
			id[i] = i;
	}
	
	void Union(int p, int q) {
		int i = root(p);
		int j = root(q);
		id[i] = j;
	}
	
	bool Connected(int p, int q) {
		return root(p) == root(q);
	}
	

private:

	int root(int i) {
		while (i != id[i]) i = id[i];
		return i;
	}

	int *id;
	int N;
};

int main() {
	ifstream fin("tinyUF.txt");
	int n;
	fin >> n;
	QuickUnionUF uf(n);
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