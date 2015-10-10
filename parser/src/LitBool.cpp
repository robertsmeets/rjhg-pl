#include "LitBool.h"

#include <iostream>

using namespace std;

LitBool::LitBool(bool){};

void LitBool::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "LitBool" << endl;
	}

}

