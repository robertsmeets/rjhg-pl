#ifndef SINGLEMETHODCALL_H_
#define SINGLEMETHODCALL_H_

#include "ExpressionList.h"

#include <string>

using namespace std;
class SingleMethodCall {
public:
	SingleMethodCall(string, ExpressionList*);
	void print(int);
};

#endif
