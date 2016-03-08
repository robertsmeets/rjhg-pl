#ifndef LITNULL_H_
#define LITNULL_H_ 

#include "Literal.h"
#include "CodeGenerator.h"

class LitNull: public Literal {
private:
   bool value;
public:
   LitNull();
   void print(int);
   virtual void emit(CodeGenerator*,ProcedureNode*);
   virtual string stype();
};

#endif
