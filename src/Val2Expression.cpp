#include "Val2Expression.h"

Val2Expression::Val2Expression(char c, Expression* lhs_e, Expression* rhs_e) {
   operation = c;
   lhs = lhs_e;
   rhs = rhs_e;
}

void Val2Expression::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("Val2Expression %c\n",operation);
   lhs->print(level + 1);
   rhs->print(level + 1);
}


void Val2Expression::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   lhs->emit(cg,pn);
   rhs->emit(cg,pn);
   cg->emitOperation(operation,NULL);
}

string Val2Expression::stype() { return "Val2Expression";}


void Val2Expression::setTopLevel() {}
