#include "LitFloat.h"

#include <iostream>

using namespace std;

LitFloat::LitFloat(double d){value = d;};

void LitFloat::print(int level)
{

	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "LitFloat" << endl;
	}

}

void LitFloat::emit(CodeGenerator* cg)
{
	int sz = sizeof(value);
	cg -> emit(1, 5, sz, NULL);
	void* loc= cg->allot(sz);
	memcpy(loc, &value, sz);
}

string LitFloat::stype() { return "LitFloat" ;}
