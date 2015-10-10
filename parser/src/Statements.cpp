#include "Statements.h"

void Statements::addStatement(Expression* s) {
	statements.push_back(s);
}

void Statements::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	for (Expression* a_statement:statements)
	{
		a_statement->print(level+1);
	}
}
