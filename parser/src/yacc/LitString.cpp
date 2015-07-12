#include "LitString.h"

#include <iostream>

using namespace std;

void LitString::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
				cout << "+";
			}
	cout << "LitString" << endl;
			}
}
