#ifndef EXPRESSIONLIST_H_
#define EXPRESSIONLIST_H_ 

#include "Expression.h"

#include <vector>
#include <iostream>

using namespace std;

class ExpressionList {
private:
	vector<Expression*> expressions;
public:
        ExpressionList();
	void addExpression(Expression*);
	void print(int);
        vector<Expression*> getExpressions();
};

#endif
