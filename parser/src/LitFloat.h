#ifndef LITFLOAT_H_
#define LITFLOAT_H_
#include "Literal.h"

class LitFloat: public Literal {
public:
	LitFloat(double);
	virtual void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
