/*
 * ProcedureNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef PROCEDURENODE_H_
#define PROCEDURENODE_H_

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

#include "Statement.h"
#include "AssignmentNode.h"
#include "PException.h"
#include "CFunctions.h"
#include "ClassDefinition.h"

using namespace std;

class ProgramNode;
// forward declaration

class Statement;
// forward declaration

class ProcedureNode {
	string name;
	ClassDefinition* cd;
	vector<string>* parameters;
	vector<string> instance_variables;
	vector<Statement*> statements;
	map<string, uint16_t>* local_variables;
	uint16_t proc_address;
	uint16_t method_number;
	ProgramNode* pn;

public:
	ProcedureNode(ClassDefinition*, string, ProgramNode*);
	virtual ~ProcedureNode();
	void setName(string);
	string getName();
	map<string, uint16_t>* getLocalVariables();
	void setProcAddress(uint16_t);
	uint16_t getProcAddress();
	vector<string>* getParameters();
	string getFullMethodName();
	void addParameter(string);
	uint16_t assignLocalVariable(string);
	uint16_t get_proc_address();
	vector<Statement*> getStatements();
	void setStatements(vector<Statement*>);
	void fixReturn();
	uint16_t getMethodNumber();
	void print(int);
	int getInstanceVarNum(string);

};

#endif /* PROCEDURENODE_H_ */
