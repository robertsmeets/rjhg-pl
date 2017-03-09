#ifndef STATEMENTS_H_
#define STATEMENTS_H_

#include <vector>
#include "stdio.h"

#include "Expression.h"

using namespace std;

class Statements {
private:
   vector<Expression*> statements;

public:
   Statements();
   void addStatement(Expression*);
   void print(int);
   vector<Expression*> getStatements();
};

#endif
