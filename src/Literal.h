#ifndef LITERAL_H_
#define LITERAL_H_

#include "Expression.h"
#include "stdio.h"

class Literal: public Expression {
	virtual void print(int);
};

#endif
