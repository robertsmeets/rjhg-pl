#ifndef SELF_H_ 
#define SELF_H_ 

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;

class Self: public Expression {
public:
   Self();
   void print(int);
   virtual void emit(CodeGenerator*, ProcedureNode*);
   virtual string stype();
   virtual void setTopLevel();
};

#endif
