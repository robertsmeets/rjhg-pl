#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_ 

#include "VariableValue.h"
#include "Expression.h"

#include <iostream>

using namespace std;

class Assignment: public Expression {
private:
	VariableValue* variable;
	Expression* expression;
public:
	Assignment(VariableValue*,Expression*);
	virtual ~Assignment();
	virtual void print(int);
	virtual void emit(CodeGenerator*);
};
#endif
