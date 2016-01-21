#include "ProcedureCall.h"

ProcedureCall::ProcedureCall(string s,ExpressionList* el)
{
   name = s;
   expressionlist = el;
}

ProcedureCall::~ProcedureCall()
{
}


void ProcedureCall::print(int level)
{
   for (int i = 0; i < level; i++) {
      cout << "+";
   }
   cout << "ProcedureCall("<<name<<")" << endl; 
   expressionlist->print(level+1);
}


void ProcedureCall::emit(CodeGenerator* cg, pProcedureNode* pn)
{
	//
	// put the parameters on the stack
	//
	/* for (vector<ExpressionNode>::iterator it = parameter_exps.begin();
			it != parameter_exps.end(); ++it) {
		cg->emitRpn((*it).getRpn(), pn, this);
	}
	if (method) {
		cg->emitRpn(LhsExpression.getRpn(), pn, this);
		cg->addCallToMethod(procedure_name, this);
	}
        else { */
		cout << "ProcedureCall::emit(" << name << endl;
		cg->addCallToProc(name, this);
}


string ProcedureCall::stype() { return "ProcedureCall" ; }
