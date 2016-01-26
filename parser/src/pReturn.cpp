#include "pReturn.h"

pReturn::pReturn(Expression* e)
{
    expression = e;
}

pReturn::~pReturn() {
}

void pReturn::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      cout << "+";
   }
   cout << "Return " << endl;
   expression->print(level+1);
}

void pReturn::emit(CodeGenerator* cg, pProcedureNode* pn)
{
   //
   // emit RET
   //
   if (expression != NULL)
   {
      expression->emit(cg,pn);
      cg->emit(2, 1, 0, NULL);
   }
   else
   {
      cg->emit(2,0,0,NULL);
   }
}

string pReturn::stype() {return "pReturn";}
