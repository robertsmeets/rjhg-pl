/*
 * ProcedureNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef PROCEDURENODE_H_
#define PROCEDURENODE_H_

#include <vector>
#include "Node.h"
#include "AssignmentNode.h"

using namespace std;

class ProcedureNode: public Node {
	string name;
	vector<string> parameters;
	vector<string> instance_variables;
	vector<AssignmentNode> assignments;

public:
	ProcedureNode();
	virtual ~ProcedureNode();
	void setName(string);
	string getName();
	void addParameter(string);
	void addInstanceVariable(string);
	void addAssignment(AssignmentNode);
	vector<AssignmentNode> getAssignments();
	unsigned int getInstanceVariable(string);
};

#endif /* PROCEDURENODE_H_ */
