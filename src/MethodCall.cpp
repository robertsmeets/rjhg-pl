#include "MethodCall.h"

#include <iostream>

using namespace std;

MethodCall::MethodCall(Expression* e, string n, ExpressionList* args)
{
   expression = e;
   name = n;
   arguments = args; 
}

MethodCall::~MethodCall()
{ }

void MethodCall::addSingleMethodCall(SingleMethodCall*) {
}

void MethodCall::print(int level) {
   {for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("MethodCall\n" );
   }
}

string MethodCall::stype() { return "MethodCall"; }

/**
 * 
 * emit bytecode for this
 *
 * It may be a procedurecall, or a class instigation, or a method call
 *
 **/
void MethodCall::emit(CodeGenerator* cg, pProcedureNode* pn)
{
   //
   // put the parameters on the stack
   //
   vector<Expression*> expressions = arguments->getExpressions();
   for (auto it=expressions.begin();it != expressions.end();++it)
   {
      (*it)->emit(cg,pn);
   }
   //
   // emit the expression
   //
   expression->emit(cg,pn);
   //
   // add a call
   //
   cg->addCallToMethod(name, NULL);
}


