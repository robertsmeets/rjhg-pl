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
   uint16_t varnum = pn->getInstanceVarNum(variable->getName());
   if (varnum != 0xffff) {
      //
      // instance variable.  emit a STI
      //
      //
      cg->emit(14, varnum, pn->getParameters()->size(), this);
   } else {
      //
      // emit a "sto" to store the value in a local variable
      //
      // stack order:
      // parameters
      // this pointer (only if in a method)
      // local variables
      // therefore if we want a local variable, we need to add the amount of parameters to it plus one
      //
      int lhs = pn->assignLocalVariable(variable->getName());
      int offset = lhs + pn->getParameters()->size() ;
      if (pn->getClassDefinition() != NULL)
      {
         offset++;
      }
      cg->emit(4, 0, offset, this);
   }
}

string Assignment::stype()
{
   return "Assignment";
}

void Assignment::setTopLevel() {}

