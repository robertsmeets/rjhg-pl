#ifndef EXPRESSION_H_
#define EXPRESSION_H_ 

#include <string>

using namespace std;

class CodeGenerator; // forward declaration

class Expression {
public:
	virtual void print(int) =0;
	virtual void emit(CodeGenerator*)=0;
	virtual string stype() = 0;
};
#endif
