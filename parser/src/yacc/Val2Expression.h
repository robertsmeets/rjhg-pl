#ifndef VAL2EXPRESSION_H_
#define VAL2EXPRESSION_H_

#include "Expression.h"

class Val2Expression:public Expression{
public:
   Val2Expression(char,Expression*, Expression*);
};

#endif