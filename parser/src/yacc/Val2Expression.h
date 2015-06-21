#ifndef VAL2EXPRESSION_H_
#define VAL2EXPRESSION_H_

#include "Expression.h"

#include <iostream>

using namespace std;

class Val2Expression: public Expression {
public:
	Val2Expression(char, Expression*, Expression*);
	void print(int);
};

#endif
