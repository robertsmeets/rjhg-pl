#include "LitString.h"

using namespace std;

LitString::LitString(string s)
{
   value = s;
}

void LitString::print(int level)
{
   for (unsigned int i = 0; i < level; i++) { printf("+"); } printf("LitString <%s>\n",value.c_str() );
}

void LitString::emit(CodeGenerator* cg, ProcedureNode* pn)
{
   int strlen = value.length();
   if (strlen <= 0)
   {
	   return;
   }
   string my_string = value.substr(0, strlen);
   cg->emit(OPCODE_LIT, TYPE_STRING, strlen, NULL);
   char* loc = cg->allot(strlen);
   memcpy(loc, my_string.c_str(), strlen);
}

string LitString::stype()
{
   return "LitString";
}

void LitString::setTopLevel() {}
