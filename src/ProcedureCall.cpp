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
      printf("+");
   }
   printf("ProcedureCall\n" );
   expressionlist->print(level+1);
}

/**
 * 
 * emit bytecode for this
 *
 * It may be a procedurecall, or a class instigation
 *
 **/
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
   pClassDefinition* cd = cg->getClassDefinition(name);
   if (cd != NULL)
   {
      cg->addCallToClassConstructor(cd,NULL);
   }
   else
   {
      cg->addCallToProcedure(name,NULL);
   }
}


string ProcedureCall::stype() { return "ProcedureCall" ; }
