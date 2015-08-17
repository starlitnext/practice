#include "TextQuery.h"
#include "QueryResult.h"
#include <iostream>
#include <fstream>

using namespace std;

void runQueries(ifstream &infile) {
	TextQuery tq(infile);
	while (true)
	{
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(cin >> s) || s == "q") break;
		print(cout, tq.query(s)) << endl;
	}
}

int main() {
	ifstream infile("query.txt", fstream::in);
	runQueries(infile);
	infile.close();
	return 0;
}