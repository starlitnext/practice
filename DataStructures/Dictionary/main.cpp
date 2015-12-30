#include <iostream>
#include <string>

#include "UALdict.h"
#include "SALdict.h"

using namespace std;

// A simple payroll entry with ID, name, address fields
class Payroll {
private:
	int ID;
	string name;
	string address;
	
public:
	Payroll(int inID, string inname, string inaddr) :
		ID(inID), name(inname), address(inaddr) { }
	~Payroll() { }
	
	int getID() const { return ID; }
	string getname() const { return name; }
	string getaddr() const { return address; }
};

ostream& operator << (ostream& os, const Payroll& payroll) {
	os << payroll.getID() << " " << payroll.getname() << " " << payroll.getaddr();
	return os;
}

int main() {
	SALdict<int, Payroll*> IDdict;
	UALdict<string, Payroll*> namedict;
	Payroll *foo1, *foo2, *findfoo1, *findfoo2;
	
	foo1 = new Payroll(5, "Joe", "Anytown");
	foo2 = new Payroll(10, "John", "Mytown");
	
	IDdict.insert(foo1->getID(), foo1);
	IDdict.insert(foo2->getID(), foo2);
	namedict.insert(foo1->getname(), foo1);
	namedict.insert(foo2->getname(), foo2);
	
	findfoo1 = IDdict.find(5);
	if (findfoo1 != NULL) cout << *findfoo1 << endl;
	else cout << "NULL" << endl;
	findfoo2 = namedict.find("John");
	if (findfoo2 != NULL) cout << *findfoo2 << endl;
	else cout << "NULL" << endl;
	
	return 0;
}