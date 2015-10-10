#include "Return.h"

Return::Return(Expression* e)
{
    expression = e;
}

Return::~Return() {
}

void Return::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Return " << endl;
	expression->print(level+1);
}
