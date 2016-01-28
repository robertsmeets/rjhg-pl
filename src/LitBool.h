#ifndef LITBOOL_H_
#define LITBOOL_H_ 

#include "Literal.h"
#include "CodeGenerator.h"

class LitBool: public Literal {
private:
	bool value;
public:
	LitBool(char*);
	void print(int);
	virtual void emit(CodeGenerator*,pProcedureNode*);
	virtual string stype();
};

#endif
