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
      cg->emitByte(OPCODE_RET);
      cg->emit2Byte(1);
   }
   else
   {
      cg->emitByte(OPCODE_RET);
      cg->emit2Byte(0);
   }
}

string ReturnNode::stype() {return "return";}


void ReturnNode::setTopLevel() {}
