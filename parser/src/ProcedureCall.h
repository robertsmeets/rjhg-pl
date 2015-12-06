#ifndef PROCEDURECALL_H_
#define PROCEDURECALL_H_

#include "ExpressionList.h"
#include "Expression.h"

#include <string>
#include <iostream>

using namespace std;

class ProcedureCall: public Expression {
private:
     string name;
     ExpressionList* expressionlist;
public:
	ProcedureCall(string,ExpressionList*);
	virtual ~ProcedureCall();
	virtual void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
