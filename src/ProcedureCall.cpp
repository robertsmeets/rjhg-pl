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
      addCallToProcedure(cg,name);
   }
}

void ProcedureCall::addCallToProcedure(CodeGenerator* cg, string procedure_name) 
{
   //
   // add room for the local variables.
   // emit an INT
   // Since we don't know how many, leave 0 for the INT parameter
   // this will be corrected in the fix stage
   //
   cg->emit(6, 0, 0, NULL);
   //
   // determine if procedure_name was defined
   // in the program code, if not it's a dynamic call
   //
   Expression* proc = cg->procDefined(procedure_name);
   if (proc != NULL) {
      //
      // emit a "cal"
      // leave the call address 0, since this will be corrected in the fix stage
      //
      cg->emit(5, 0, 0, NULL);
      cg->addCallAddress(cg->getHere() - 2, procedure_name);
   } else {
      //
      // dealing with a dynamic call, to a library function
      // look up the external function
      //
      Extern* external = cg->getProgramNode()->lookupExternal(procedure_name);
      cg->emit(10, external->getNumber(), 0, NULL);
   }
}


string ProcedureCall::stype() { return "ProcedureCall" ; }
