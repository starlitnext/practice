#include <iostream>
#include "Vector.h"

using namespace std;

int main() {
	Vector<double> v1(10, 0);
	cout << v1.size() << endl;
	cout << v1.capacity() << endl;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	cout << v1.size() << endl;
	cout << v1.capacity() << endl;
	v1.resize(20);
	cout << v1.size() << endl;
	cout << v1.capacity() << endl;
	Vector<double> v2(v1);
	for (auto it = v2.begin(); it != v2.end(); it++)
		cout << *it << " ";
	cout << endl;
	
	return 0;
}