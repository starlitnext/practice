#include <iostream>

#include "AList.h"
#include "LList.h"

using namespace std;

int main() {
	
	//AList<int> list;
	LList<int> list;
	for (int i = 0; i < 10; i++)
		list.append(i);
	
	cout << list << endl;
	
	list.moveToStart();
	list.insert(100);
	
	cout << list << endl;
	
	list.moveToPos(2);
	cout << list.remove() << endl;
	cout << list << endl;
	
	return 0;
}
