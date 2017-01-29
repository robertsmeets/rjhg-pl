#ifndef NOTEXPRESSION_H_
#define NOTEXPRESSION_H_

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;

class NotExpression: public Expression {
private:
   Expression* rhs;
public:
   NotExpression(Expression*);
   void print(int);
   virtual void emit(CodeGenerator*, ProcedureNode*);
   virtual string stype();
   virtual void setTopLevel();
};

#endif
