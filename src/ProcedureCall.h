#ifndef PROCEDURECALL_H_
#define PROCEDURECALL_H_

#include "ExpressionList.h"
#include "Expression.h"
#include "CodeGenerator.h"

#include <string>
#include "stdio.h"

using namespace std;

class ProcedureCall: public Expression {
private:
     string name;
     ExpressionList* expressionlist;
public:
   ProcedureCall(string,ExpressionList*);
   virtual ~ProcedureCall();
   virtual void print(int);
   virtual void emit(CodeGenerator*,pProcedureNode*);
   virtual string stype();
};

#endif
