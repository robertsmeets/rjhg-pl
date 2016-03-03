#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_ 

#include "Expression.h"
#include "VariableValue.h"
#include "stdio.h"
#include "ProcedureNode.h"

using namespace std;

class Assignment: public Expression {
private:
   VariableValue* variable;
   Expression* expression;
public:
   Assignment(VariableValue*,Expression*);
   virtual ~Assignment();
   void print(int);
   virtual void emit(CodeGenerator*,ProcedureNode*);
   virtual string stype();
};
#endif
