#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <iostream>

using namespace std;

class Statement {
public:
	virtual ~Statement();
	virtual void print(int) = 0;
};

#endif

