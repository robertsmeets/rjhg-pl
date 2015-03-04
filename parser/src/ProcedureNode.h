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
#include "ReturnNode.h"

using namespace std;

class ProgramNode; // forward declaration

class Statement; // forward declaration

class ProcedureNode {
	string name;
	vector<string>* parameters;
	vector<string> instance_variables;
	vector<Statement*>* statements;
	map<string,unsigned int>* local_variables;
	unsigned int proc_address;

public:
	ProcedureNode();
	virtual ~ProcedureNode();
	void setName(string);
	string getName();
	map<string,unsigned int>* getLocalVariables();
	void setProcAddress(unsigned int );
	unsigned int getProcAddress();
	vector<string>* getParameters();
	void addParameter(string);
	unsigned int assignLocalVariable(string);
	unsigned int get_proc_address();
	vector<Statement*>* getStatements();
	void setStatements(vector<Statement*>*);
	void fixReturn();
};

#endif /* PROCEDURENODE_H_ */
