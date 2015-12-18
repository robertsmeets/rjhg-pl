#ifndef COMPOSITEMETHODCALL_H_
#define COMPOSITEMETHODCALL_H_ 

#include "Expression.h"
#include "SingleMethodCall.h"

class CompositeMethodCall: public Expression {
public:
	virtual ~CompositeMethodCall();
	void addSingleMethodCall(SingleMethodCall*);
	virtual void print(int);
	virtual void emit(CodeGenerator*);
	virtual string stype();

};
#endif
