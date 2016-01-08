#ifndef WHILE_H_
#define WHILE_H_ 

#include "Statements.h"
#include "Expression.h"

#include <iostream>
#include <vector>

using namespace std;

class While: public Expression {

private:
	Expression* expression;
        Statements* statements;
public:
	While(Expression*,Statements*);
	virtual ~While();
	virtual void print(int);
	virtual void emit(CodeGenerator*,pProcedureNode*);
	virtual string stype();
};
#endif
