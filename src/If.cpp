#include "If.h"

If::If(Expression* e,Statements* s1, Statements* s2)
{
    expression = e;
    statements1 = s1;
    statements2 = s2;
}

If::~If() {
}

void If::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("If\n" );
   expression->print(level+1);
        statements1->print(level+1);
        if (statements2 != NULL)
        {
           statements2->print(level+1);
        }
}


void If::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   //
   // emit the instructions to calculate the value and put it on the stack
   //
   expression->emit(cg,pn);
        vector<Expression*> vstatements1 = statements1->getStatements();
        if (statements2 == NULL) {
      //
      // emit JPC, jump if the stack top is true
      //
      cg->emit(8, 0, 0, this);
      unsigned int jump_address = cg->getHere() - 2;
      for (auto it = vstatements1.begin(); it != vstatements1.end(); ++it) {
         (*it)->emit(cg,pn);
      }
      unsigned int dest_address = cg->getHere();
      cg->fix(jump_address, dest_address);
   } else {
      cg->emit(8, 0, 0, this);
      unsigned int jump_address1 = cg->getHere() - 2;
                vector<Expression*> vstatements2 = statements2->getStatements();
      for (auto it = vstatements2.begin(); it != vstatements2.end(); ++it) {
         (*it)->emit(cg,pn);
      }
      //
      // jump
      //
      cg->emit(7, 0, 0, this);
      unsigned int jump_address2 = cg->getHere() - 2;
      unsigned int dest_address1 = cg->getHere();
      for (auto it = vstatements1.begin(); it != vstatements1.end(); ++it) {
         (*it)->emit(cg,pn);
      }
      unsigned int dest_address2 = cg->getHere();
      cg->fix(jump_address1, dest_address1);
      cg->fix(jump_address2, dest_address2);
   }
}

string If::stype() {
   return "if";
}


void If::setTopLevel() {}

