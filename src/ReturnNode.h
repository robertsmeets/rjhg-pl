#ifndef RETURN_H_
#define RETURN_H_ 

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;


class ReturnNode: public Expression {

private:
	Expression* expression;
public:
	ReturnNode(Expression*);
	virtual ~ReturnNode();
	virtual void print(int);
	virtual void emit(CodeGenerator*,ProcedureNode*);
	virtual string stype();
};
#endif
