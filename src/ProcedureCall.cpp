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
   vector<Expression*> expressions = expressionlist->getExpressions();
   for (auto it=expressions.begin();it != expressions.end();++it)
   {
      (*it)->emit(cg,pn);
   }

   /* 

   if (method) {
      cg->emitRpn(LhsExpression.getRpn(), pn, this);
      cg->addCallToMethod(procedure_name, this);
   } */
      cg->addCallToProcedure(name,NULL);
}


string ProcedureCall::stype() { return "ProcedureCall" ; }
