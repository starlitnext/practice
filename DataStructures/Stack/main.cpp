#include <iostream>

#include "AStack.h"
#include "LStack.h"

using namespace std;

int main() {
	//AStack<int> stack;
	LStack<int> stack;
	
	for (int i = 0; i < 10; i++)
		stack.push(i);
	
	cout << stack.length() << endl;
	
	while (stack.length() != 0) {
		cout << stack.top();
		cout << stack.pop() << " ";
	}
	cout << endl;
	
	return 0;
}