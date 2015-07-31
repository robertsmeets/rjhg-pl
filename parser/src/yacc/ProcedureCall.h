#ifndef PROCEDURECALL_H_
#define PROCEDURECALL_H_

#include "Statement.h"
#include "ExpressionList.h"
#include "Expression.h"

class ProcedureCall: public Statement, public Expression {
private:
     string name;
     ExpressionList* expressionlist;
public:
	ProcedureCall(string,ExpressionList*);
	virtual ~ProcedureCall();
	virtual void print(int);
};

#endif
