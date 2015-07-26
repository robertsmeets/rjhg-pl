#ifndef IF_H_
#define IF_H_ 

#include "Statements.h"
#include "Expression.h"

#include <iostream>

using namespace std;

class If: public Statement {

private:
	Expression* expression;
        Statements* statements1;
        Statements* statements2;
public:
	If(Expression*, Statements*, Statements*);
	virtual ~If();
	virtual void print(int);
};
#endif
