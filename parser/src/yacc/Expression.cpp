#include "Expression.h"

#include <iostream>

using namespace std;

void Expression::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Expression" << endl;
	}
}

