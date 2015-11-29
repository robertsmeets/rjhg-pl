#ifndef EXPRESSION_H_
#define EXPRESSION_H_ 

#include "CodeGenerator.h"

class Expression {
public:
	virtual void print(int) =0;
	void emit(CodeGenerator*);
};
#endif
