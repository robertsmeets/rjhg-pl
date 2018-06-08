#include "LitNull.h"

using namespace std;

LitNull::LitNull()
{
}

void LitNull::print(int level)
{
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("LitNull\n");
}

void LitNull::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   cg->emit(OPCODE_LIT, 0, 0, NULL);
}

string LitNull::stype() { return "LitNull"; }


void LitNull::setTopLevel() {}
