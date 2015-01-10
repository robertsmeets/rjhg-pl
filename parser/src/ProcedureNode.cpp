/*
 * ProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProcedureNode.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "PException.h"

using namespace std;

ProcedureNode::ProcedureNode() {
	name = "";
	parameters = vector<string>();
	instance_variables = vector<string>();
	statements = vector<Statement*>();
}

ProcedureNode::~ProcedureNode() {
	//
	// delete all the statements
	//

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

void ProcedureNode::addStatement(Statement* s) {
	statements.push_back(s);
}

vector<Statement*> ProcedureNode::getStatements() {
	return statements;
}

/**
 * find the index of an index variable by string
 *
 */
unsigned int ProcedureNode::getInstanceVariable(string s) {
	unsigned int i = find(instance_variables.begin(), instance_variables.end(),
			s) - instance_variables.begin();
	if (i == instance_variables.size()) {
		PException("instance variable " + s + " not found");
	}
	return i;

}

