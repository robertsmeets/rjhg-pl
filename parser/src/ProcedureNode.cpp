/*
 * ProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProcedureNode.h"

using namespace std;

ProcedureNode::ProcedureNode() {
	name = "";
	parameters.clear();
	instance_variables.clear();
	assignments.clear();
}

ProcedureNode::~ProcedureNode() {

}

void ProcedureNode::setName(string a_name) {
	name = a_name;
}

string ProcedureNode::getName() {
	return name;
}

void ProcedureNode::addParameter(string a_parameter) {
	parameters.push_back(a_parameter);
}

void ProcedureNode::addInstanceVariable(string an_instance_variable) {
	instance_variables.push_back(an_instance_variable);
}

void ProcedureNode::addAssignment(AssignmentNode an) {
	assignments.push_back(an);
}

vector<AssignmentNode> ProcedureNode::getAssignments()
{
	return assignments;
}
