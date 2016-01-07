#ifndef RETURN_H_
#define RETURN_H_ 

#include "Expression.h"
#include "CodeGenerator.h"

#include <iostream>

using namespace std;

class pReturn: public Expression {

private:
	Expression* expression;
public:
	pReturn(Expression*);
	virtual ~pReturn();
	virtual void print(int);
	virtual void emit(CodeGenerator*);
	virtual string stype();
};
#endif
