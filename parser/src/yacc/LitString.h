#ifndef LITSTRING_H_
#define LITSTRING_H_

#include "Literal.h"

#include <string>

using namespace std;

class LitString: public Literal {
public:
	LitString (string);
	virtual void print(int);
};

#endif
