#ifndef RETURN_H_
#define RETURN_H_ 

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;


class pReturn: public Expression {

private:
	Expression* expression;
public:
	pReturn(Expression*);
	virtual ~pReturn();
	virtual void print(int);
	virtual void emit(CodeGenerator*,pProcedureNode*);
	virtual string stype();
};
#endif
