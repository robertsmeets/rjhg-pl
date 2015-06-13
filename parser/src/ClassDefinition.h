/*
 * ClassDefinition.h
 *
 *  Created on: Mar 23, 2015
 *      Author: robert
 */

#ifndef CLASSDEFINITION_H_
#define CLASSDEFINITION_H_

#include <string>
#include <vector>

#include "ProcedureNode.h"

using namespace std;

class ProcedureNode;
// forward declaration

class ClassDefinition {
	string name;
	unsigned int classnum;
	vector<string> instance_variables;
	vector<ProcedureNode*> methods;

public:
	ClassDefinition(string, unsigned int);
	virtual ~ClassDefinition();
	string getName();
	void add_instance_variable(string);
	void add_method(ProcedureNode*);
	vector<string> getInstanceVariables();
	unsigned int getClassNum();
	vector<ProcedureNode*> getMethods();
	ProcedureNode* findMethod(string);
	void print(unsigned int);
};

#endif /* CLASSDEFINITION_H_ */
