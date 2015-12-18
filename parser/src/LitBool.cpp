#include "LitBool.h"

#include <iostream>

using namespace std;

LitBool::LitBool(bool b){value = b;};

void LitBool::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "LitBool" << endl;
	}

}

void LitBool::emit(CodeGenerator* cg)
{
	cg->emit(1, 6, value, NULL);
}

string LitBool::stype() { return "LitBool"; }
