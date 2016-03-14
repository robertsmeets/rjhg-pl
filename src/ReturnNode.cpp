#include "ReturnNode.h"

ReturnNode::ReturnNode(Expression* e)
{
    expression = e;
}

ReturnNode::~ReturnNode() {
}

void ReturnNode::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("Return\n" );
   if (expression != NULL)
   {
      expression->print(level+1);
   }
}

void ReturnNode::emit(CodeGenerator* cg, ProcedureNode* pn)
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

string ReturnNode::stype() {return "return";}


void ReturnNode::setTopLevel() {}
