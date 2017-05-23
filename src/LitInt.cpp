#include "LitInt.h"

using namespace std;

LitInt::LitInt(char* a_string)
{
   value = atoi(a_string);

};

void LitInt::print(int level)
{
   for (unsigned int i = 0; i < level; i++) {
      printf("+");
   }
   printf("LitInt %d\n" , value );
}


void LitInt::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   cg -> emit(1, 2, value, NULL);
}

string LitInt::stype() { return "LitInt"; }

void LitInt::setTopLevel() {}
