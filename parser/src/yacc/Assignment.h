#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_ 

#include "Statement.h"

#include <iostream>

using namespace std;

class Assignment: public Statement {
	virtual ~Assignment();
	virtual void print(int);
};
#endif
