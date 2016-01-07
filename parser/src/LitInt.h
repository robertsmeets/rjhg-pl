#ifndef LITINT_H_
#define LITINT_H_

#include "Literal.h"
#include "CodeGenerator.h"

class LitInt: public Literal {
private:
	int value;
public:
	LitInt(char*);
	virtual void print(int);
	virtual void emit(CodeGenerator*);
	virtual string stype();
};

#endif
