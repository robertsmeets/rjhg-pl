#ifndef METHODCALL_H_
#define METHODCALL_H_ 

#include "Expression.h"
#include "SingleMethodCall.h"

class MethodCall: public Expression {
private:
   Expression* expression;
   string name;
   ExpressionList* arguments;
public:
   MethodCall(Expression*,string,ExpressionList*);
   virtual ~MethodCall();
   void addSingleMethodCall(SingleMethodCall*);
   virtual void print(int);
   virtual void emit(CodeGenerator*,pProcedureNode*);
   virtual string stype();

};
#endif
