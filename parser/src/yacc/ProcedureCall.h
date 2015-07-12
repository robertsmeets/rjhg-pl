#ifndef PROCEDURECALL_H_
#define PROCEDURECALL_H_

#include "Statement.h"
#include "Expression.h"

class ProcedureCall: public Statement, public Expression {
	virtual ~ProcedureCall();
	virtual void print(int);
};

#endif
