#include "Self.h"

Self::Self() {
}

void Self::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("Self\n");
}


void Self::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   int nr = pn->getParameters()->size();
   cg->emit(OPCODE_SLF,nr,0,NULL);
}

string Self::stype() { return "Self";}


void Self::setTopLevel() {}
