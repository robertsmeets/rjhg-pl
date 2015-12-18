#include "Assignment.h"

Assignment::Assignment(VariableValue* v,Expression* e)
{
    variable = v;
    expression = e;
}

Assignment::~Assignment() {
}

void Assignment::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Assignment " << endl;
	variable->print(level+1);
	expression->print(level+1);
}

void Assignment::emit(CodeGenerator* cg)
{

}

string Assignment::stype()
{
   return "Assignment";
}
