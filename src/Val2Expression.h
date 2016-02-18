#ifndef VAL2EXPRESSION_H_
#define VAL2EXPRESSION_H_

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;

class Val2Expression: public Expression {
private:
	char operation;
	Expression* lhs;
	Expression* rhs;
public:
	Val2Expression(char, Expression*, Expression*);
	void print(int);
	virtual void emit(CodeGenerator*, pProcedureNode*);
	virtual string stype();
};

#endif
