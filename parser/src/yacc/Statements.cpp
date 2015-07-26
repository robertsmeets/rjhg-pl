#include "Statements.h"

void Statements::addStatement(Statement* s) {
	statements.push_back(s);
}

void Statements::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	for (Statement* a_statement:statements)
	{
		a_statement->print(level+1);
	}
}
