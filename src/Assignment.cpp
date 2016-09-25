#include "Assignment.h"

Assignment::Assignment(VariableValue* v,Expression* e)
{
    variable = v;
    expression = e;
}

Assignment::~Assignment() {
}

void Assignment::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("Assignment\n" );
   variable->print(level+1);
   expression->print(level+1);
}

void Assignment::emit(CodeGenerator* cg, ProcedureNode* pn) {
   //
   // emit the calculation instructions
   //
   expression->emit(cg, pn);
   int varnum = pn->getInstanceVarNum(variable->getName());
   if (varnum > 0) {
      //
      // instance variable.  emit a STOI
      //
      cg->emit(14, varnum, 0, this);
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

void Assignment::setTopLevel() {}

