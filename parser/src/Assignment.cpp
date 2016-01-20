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

void Assignment::emit(CodeGenerator* cg, pProcedureNode* pn) {
	//
	// emit the calculation instructions
	//
   	expression->emit(cg, pn);
	bool is_iv = false;
	if (is_iv) {
		//
		// instance variable.  emit a STOI
		//
		//cout << "void AssignmentNode::emit(CodeGenerator* cg) STOI " << lhs << endl;
		//cg->emit(14, lhs, 0, this);
	} else {
		//
		// emit a "sto" to store the value in a local variable
		//
		// stack order:
		// parameters
		// local variables
		// therefore if we want a local variable, we need to add the amount of parameters to it
		//
		int lhs = pn->assignLocalVariable(variable->getName());
		cg->emit(4, 0, lhs + pn->getParameters()->size(), this);
	}
}

string Assignment::stype()
{
   return "Assignment";
}

