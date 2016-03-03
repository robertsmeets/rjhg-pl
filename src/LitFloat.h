#ifndef LITFLOAT_H_
#define LITFLOAT_H_

#include "Literal.h"
#include "CodeGenerator.h"

class LitFloat: public Literal {

private:
	double value;
public:
	LitFloat(char*);
	virtual void print(int);
	virtual void emit(CodeGenerator*,ProcedureNode*);
	virtual string stype();
};

#endif
