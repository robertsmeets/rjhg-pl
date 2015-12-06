#ifndef LITINT_H_
#define LITINT_H_

#include "Literal.h"

class LitInt: public Literal {
private:
	int value;
public:
	LitInt(int);
	virtual void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
