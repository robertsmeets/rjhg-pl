#ifndef WHILE_H_
#define WHILE_H_ 

#include "Statements.h"
#include "Expression.h"

#include <iostream>

using namespace std;

class While: public Statement {

private:
	Expression* expression;
        Statements* statements;
public:
	While(Expression*,Statements*);
	virtual ~While();
	virtual void print(int);
};
#endif
