#include "Statements.h"

void Statements::addStatement(Expression* s) {
	if (s!=NULL)
        {
           statements.push_back(s);
        }
}

void Statements::print(int level) {
	for (int i = 0; i < level; i++) {
		cout << "+";
	}
   cout << "Statements" << endl;
	for (Expression* a_statement:statements)
	{
		a_statement->print(level+1);
	}
}

vector<Expression*> Statements::getStatements()
{
	return statements;
}
