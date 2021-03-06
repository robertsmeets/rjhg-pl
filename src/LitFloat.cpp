#include "LitFloat.h"

using namespace std;

LitFloat::LitFloat(char* s)
{
   value = atof(s);
}

void LitFloat::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
		printf("+");
	}
	printf("LitFloat %f\n" , value );
	}

}

void LitFloat::emit(CodeGenerator* cg, ProcedureNode* pn)
{
	int sz = sizeof(value);
	cg -> emit(OPCODE_LIT, TYPE_FLOAT, sz, NULL);
	void* loc= cg->allot(sz);
	memcpy(loc, &value, sz);
}

string LitFloat::stype() { return "LitFloat" ;}

void LitFloat::setTopLevel() {}
