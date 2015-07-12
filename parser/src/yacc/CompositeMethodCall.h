#ifndef COMPOSITEMETHODCALL_H_
#define COMPOSITEMETHODCALL_H_ 

#include "Statement.h"
#include "Expression.h"
#include "SingleMethodCall.h"

class CompositeMethodCall: public Statement, public Expression {
public:
	virtual ~CompositeMethodCall();
	void addSingleMethodCall(SingleMethodCall*);
	virtual void print(int);

};
#endif
