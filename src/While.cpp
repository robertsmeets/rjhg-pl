#include "While.h"

While::While(Expression* e,Statements* s)
{
    expression = e;
    statements = s;
}

While::~While() {
}

void While::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("While\n" );
   expression->print(level+1);
        statements->print(level+1);
}

void While::emit(CodeGenerator* cg, ProcedureNode* pn)
{

   //
   // emit the instructions to calculate the value and put it on the stack
   //
   unsigned int dest_address1 = cg->getHere();
   expression->emit(cg,pn);
   //
   // emit JPF, jump if the stack top is 0 (false)
   //
   cg->emit(17, 0, 0, this);
   unsigned int jump_address2 = cg->getHere() - 2;
   //
   // emit the block statements
   //
   vector<Expression*> expressions = statements->getStatements();
        for (auto it = expressions.begin(); it != expressions.end(); ++it) {
      (*it)->emit(cg,pn);
   }
   //
   // jump back to the dest_address
   //
   cg->emit(7, 0, 0, this);
   unsigned int dest_address2 = cg->getHere();
   unsigned int jump_address1 = cg->getHere() - 2;
   cg->fix(jump_address1, dest_address1);
   cg->fix(jump_address2, dest_address2);

}

string While::stype() {
   return "while";
}

void While::setTopLevel() {}
