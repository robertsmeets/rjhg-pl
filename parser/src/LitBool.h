#ifndef LITBOOL_H_
#define LITBOOL_H_ 

#include "Literal.h"

class LitBool: public Literal {
private:
	bool value;
public:
	LitBool(bool);
	void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
