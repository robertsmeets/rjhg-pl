#ifndef EXPRESSION_H_
#define EXPRESSION_H_ 

#include <string>

using namespace std;


class ProcedureNode; // forward declaration


class CodeGenerator; // forward declaration

class Expression {
public:
   virtual void print(int) =0;
   virtual void emit(CodeGenerator*, ProcedureNode*)=0;
   virtual string stype() = 0;
   virtual void setTopLevel() = 0;
};
#endif
