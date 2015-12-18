#include "pReturn.h"

pReturn::pReturn(Expression* e)
{
    expression = e;
}

pReturn::~pReturn() {
}

void pReturn::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Return " << endl;
	expression->print(level+1);
}

void pReturn::emit(CodeGenerator* cg)
{

}

string pReturn::stype() {return "pReturn";}
