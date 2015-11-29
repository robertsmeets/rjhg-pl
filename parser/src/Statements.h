#ifndef STATEMENTS_H_
#define STATEMENTS_H_

#include "Expression.h"

#include <vector>
#include <iostream>

using namespace std;

class Expression; //forward declaration

class Statements {
private:
	vector<Expression*> statements;
public:
	void addStatement(Expression*);
	void print(int);
	vector<Expression*> getStatements();
};

#endif
