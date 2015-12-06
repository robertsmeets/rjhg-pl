#include "LitInt.h"

#include <iostream>

using namespace std;

LitInt::LitInt(int){};

void LitInt::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
				cout << "+";
			}
		cout << "LitInt" << endl;
			}

}


void LitInt::emit(CodeGenerator* cg)
{

}
