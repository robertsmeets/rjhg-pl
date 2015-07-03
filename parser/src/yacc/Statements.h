#ifndef STATEMENTS_H_
#define STATEMENTS_H_

#include "Statement.h"

#include <vector>

class Statements {
private:
	vector<Statement*> statements;
public:
	void addStatement(Statement*);
	void print(int);
};

#endif
