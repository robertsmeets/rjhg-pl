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
	//
		// emit RET
		//
		cg->emit(2, 0, 0, NULL);
}

string pReturn::stype() {return "pReturn";}
