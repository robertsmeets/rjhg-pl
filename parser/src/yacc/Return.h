#ifndef RETURN_H_
#define RETURN_H_ 

#include "Expression.h"

#include <iostream>

using namespace std;

class Return: public Expression {

private:
	Expression* expression;
public:
	Return(Expression*);
	virtual ~Return();
	virtual void print(int);
};
#endif
