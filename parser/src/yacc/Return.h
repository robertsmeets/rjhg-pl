#ifndef RETURN_H_
#define RETURN_H_ 

#include "Statement.h"
#include "Expression.h"

#include <iostream>

using namespace std;

class Return: public Statement {

private:
	Expression* expression;
public:
	Return(Expression*);
	virtual ~Return();
	virtual void print(int);
};
#endif
