#include "NotExpression.h"

NotExpression::NotExpression(Expression* rhs_e) {
   rhs = rhs_e;
}

void NotExpression::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("NotExpression");
   rhs->print(level + 1);
}

void NotExpression::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   rhs->emit(cg,pn);
   cg->emitOperation('!',NULL);
}

string NotExpression::stype() { return "NotExpression";}

void NotExpression::setTopLevel() {}
