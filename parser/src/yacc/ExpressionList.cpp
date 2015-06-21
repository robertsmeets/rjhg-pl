#include "ExpressionList.h"

#include <iostream>

using namespace std;

void ExpressionList::addExpression(Expression* e)
{

}

void ExpressionList::print(int level)
{
	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "ExpressionList" << endl;
	}
}

