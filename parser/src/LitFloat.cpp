#include "LitFloat.h"

#include <iostream>

using namespace std;

LitFloat::LitFloat(double){};

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

}
