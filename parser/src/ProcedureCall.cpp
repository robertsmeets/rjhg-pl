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
   for (unsigned int i = 0; i < level; i++) {
	 						cout << "+";
	 					}
   cout << "ProcedureCall("<<name<<")" << endl; 
   expressionlist->print(level+1);
}
