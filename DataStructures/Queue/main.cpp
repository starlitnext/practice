#include <iostream>

#include "AQueue.h"
#include "LQueue.h"

using namespace std;

int main() {
	LQueue<int> queue;
	for (int i = 0; i < 10; i++)
		queue.enqueue(i);
	while (queue.length() != 0) {
		cout << queue.frontValue();
		cout << queue.dequeue() << " ";
	}
	cout << endl;
	
	return 0;
}