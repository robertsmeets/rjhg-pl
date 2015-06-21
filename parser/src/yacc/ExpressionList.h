#ifndef EXPRESSIONLIST_H_
#define EXPRESSIONLIST_H_ 

#include "Expression.h"

#include <vector>

using namespace std;

class ExpressionList {

	vector<Expression> expressions;
public:
	void addExpression(Expression*);
	void print(int);
};

#endif
