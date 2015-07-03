#ifndef PMETHODDEFINITION_H_
#define PMETHODDEFINITION_H_
#include <string>

#include "Statements.h"

using namespace std;

class pMethodDefinition {
	string class_name;
	string method_name;
	Statements* statements;
public:
	pMethodDefinition(string,string,Statements*);
	virtual ~pMethodDefinition();
	void print(int);
};

#endif
