#include "LitInt.h"

#include <iostream>

using namespace std;

LitInt::LitInt(int i)
{
	value = i;
};

void LitInt::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
				cout << "+";
			}
		cout << "LitInt " << value << endl;
			}

}


void LitInt::emit(CodeGenerator* cg)
{
	cg -> emit(1, 2, value, NULL);
}
