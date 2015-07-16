#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_ 

#include "Statement.h"
#include "VariableValue.h"
#include "Expression.h"

#include <iostream>

using namespace std;

class Assignment: public Statement {
private:
	VariableValue* variable;
	Expression* expression;
public:
	Assignment(VariableValue*,Expression*);
	virtual ~Assignment();
	virtual void print(int);
};
#endif
