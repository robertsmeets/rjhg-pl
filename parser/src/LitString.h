#ifndef LITSTRING_H_
#define LITSTRING_H_

#include "Literal.h"

#include <string>

using namespace std;

class LitString: public Literal {
private:
	string value;
public:
	LitString (string);
	virtual void print(int);
	virtual void emit(CodeGenerator*);
};

#endif
