/*
 * ProgramNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef PROGRAMNODE_H_
#define PROGRAMNODE_H_

#include <vector>

#include "ProcedureNode.h"
#include "ClassDefinition.h"

using namespace std;

class ProcedureNode;
// forward declaration
class ClassDefinition;
// forward declaration

class ProgramNode {
	vector<ProcedureNode*> procedures;
	vector<ClassDefinition*> classes;
	map<string, unsigned int> method_numbers;
	unsigned int next_methodnumber;
public:
	ProgramNode();
	virtual ~ProgramNode();
	void addProcedure(ProcedureNode*);
	void addClass(ClassDefinition*);
	vector<ProcedureNode*> getProcedures();
	vector<ClassDefinition*> getClasses();
	ClassDefinition* getClass(string);
	unsigned int assignMethodNumber(string);
	unsigned int getMethodNumber(string);
	void print();
};

#endif /* PROGRAMNODE_H_ */
