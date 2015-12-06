#ifndef LITBOOL_H_
#define LITBOOL_H_ 

#include "Literal.h"

class LitBool: public Literal {
public:
	LitBool(bool);
	void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
