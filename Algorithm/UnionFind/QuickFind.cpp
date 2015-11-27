#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class QuickFindUF {
public:
	explicit QuickFindUF(int n) : N(n) { 
		id = new int[N]; 
		for (int i = 0; i < N; i++)
			id[i] = i;
	}
	~QuickFindUF() { delete [] id; }
	
	void Union(int p, int q) {
		int pid = id[p];
		int qid = id[q];
		for (int i = 0; i < N; i++)
			if (id[i] == pid) id[i] = qid;
	}
	
	bool Connected(int p, int q) { return id[p] == id[q]; }
	
private:
	int *id;
	int N;
};

int main() {
	ifstream fin("tinyUF.txt");
	int n;
	fin >> n;
	QuickFindUF uf(n);
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