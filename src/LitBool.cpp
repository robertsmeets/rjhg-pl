#include "LitBool.h"

#include <iostream>

using namespace std;

LitBool::LitBool(char* s)
{
   if (strcmp(s,"true")==0 )
   {
      value = true;
   }
   else
   {
      if (strcmp(s,"false")==0 )
      {
          value = false;
      }
      else
      {
          puts("boolean must be true or false");
      }
   }
}

void LitBool::print(int level)
{
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("LitBool\n" );
   

}

void LitBool::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   cg->emit(1, 6, value, NULL);
}

string LitBool::stype() { return "LitBool"; }


void LitBool::setTopLevel() {}
