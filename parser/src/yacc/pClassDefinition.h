#ifndef PCLASSDEFINITION_H_
#define PCLASSDEFINITION_H_

#include <string>
#include <iostream>

using namespace std;

class pClassDefinition {
private:
	string name;
public:
	pClassDefinition(string);
	virtual ~pClassDefinition();
	void print(int);
	string getName();
};

#endif
