#include "LitString.h"

#include <iostream>

using namespace std;

LitString::LitString(string s)
{
   value = s;
}

void LitString::print(int level)
{
   {for (unsigned int i = 0; i < level; i++) {
            printf("+");
         }
   printf("LitString\n" );
         }
}

void LitString::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   int strlen = value.length() - 2;
   string my_string = value.substr(1, strlen);
   cg->emit(1, 7, strlen, NULL);
   char* loc = cg->allot(strlen);
   memcpy(loc, my_string.c_str(), strlen);
}

string LitString::stype()
{
   return "LitString";
}


void LitString::setTopLevel() {}
