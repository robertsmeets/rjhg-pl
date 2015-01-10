/*
 * ProcedureNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef PROCEDURENODE_H_
#define PROCEDURENODE_H_

#include <vector>
#include "Statement.h"
#include "AssignmentNode.h"

using namespace std;

class ProgramNode; // forward declaration

class Statement; // forward declaration

class ProcedureNode {
	string name;
	vector<string> parameters;
	vector<string> instance_variables;
	vector<Statement*> statements;

public:
	ProcedureNode();
	virtual ~ProcedureNode();
	void setName(string);
	string getName();
	void addParameter(string);
	void addInstanceVariable(string);
	void addStatement(Statement*);
	vector<Statement*> getStatements();
	unsigned int getInstanceVariable(string);

};

#endif /* PROCEDURENODE_H_ */
