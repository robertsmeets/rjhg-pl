#include "While.h"

While::While(Expression* e,Statements* s)
{
    expression = e;
    statements = s;
}

While::~While() {
}

void While::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "While " << endl;
	expression->print(level+1);
        statements->print(level+1);
}
