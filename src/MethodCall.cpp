#include "MethodCall.h"

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
   for (unsigned int i = 0; i < level; i++) { printf("+"); }
   printf("MethodCall %s\n",name.c_str());
   expression->print(level+1);
   arguments->print(level+1);
}

string MethodCall::stype() { return "MethodCall"; }

/**
 * 
 * emit bytecode for this
 *
 **/
void MethodCall::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   //
   // put the parameters on the stack
   //
   vector<Expression*> expressions = arguments->getExpressions();
   for (auto it=expressions.begin();it != expressions.end();++it)
   {
      (*it)->emit(cg, pn);
   }
   //
   // emit the expression
   //
   expression->emit(cg, pn);
   //
   // add a call
   //
   cg->emit(12,cg->getProgramNode()->getMethodNumber(name),expressions.size(),NULL);
}


void MethodCall::setTopLevel() {}
