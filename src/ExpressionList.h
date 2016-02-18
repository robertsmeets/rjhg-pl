#ifndef EXPRESSIONLIST_H_
#define EXPRESSIONLIST_H_ 

#include "Expression.h"

#include "stdio.h"
#include <vector>

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
