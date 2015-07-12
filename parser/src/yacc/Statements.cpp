#include "Statements.h"

void Statements::addStatement(Statement* s) {
	cout << "Statements::addStatement()" << endl;
	statements.push_back(s);
	cout << "statements.size is " << statements.size() << endl;
}

void Statements::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Statements there are " << statements.size() << endl;
	for (Statement* a_statement:statements)
	{
		a_statement->print(level+1);
	}
}
