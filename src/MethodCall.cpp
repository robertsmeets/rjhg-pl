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
      cout << "+";
   }
   cout << "MethodCall" << endl;
   }
}

void MethodCall::emit(CodeGenerator* cg, pProcedureNode* pn)
{

}

string MethodCall::stype() { return "MethodCall"; }

