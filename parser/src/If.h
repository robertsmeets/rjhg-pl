#ifndef IF_H_
#define IF_H_ 

#include "Statements.h"
#include "Expression.h"
#include "CodeGenerator.h"

#include <iostream>

using namespace std;

class If: public Expression {

private:
	Expression* expression;
        Statements* statements1;
        Statements* statements2;
public:
	If(Expression*, Statements*, Statements*);
	virtual ~If();
	virtual void print(int);
	virtual void emit(CodeGenerator*, pProcedureNode*);
	virtual string stype();
};
#endif
