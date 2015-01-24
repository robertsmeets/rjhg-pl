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
	parameters = vector<string>();
	instance_variables = vector<string>();
	local_variables = new map<string, unsigned int>;
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

void ProcedureNode::setProcAddress(unsigned int a) {
	proc_address = a;
}

unsigned int ProcedureNode::getProcAddress() {
	return proc_address;
}

vector<string> ProcedureNode::getParameters() {
	return parameters;
}

map<string, unsigned int>* ProcedureNode::getLocalVariables() {
	return local_variables;
}

void ProcedureNode::addParameter(string a_parameter) {
	parameters.push_back(a_parameter);
}

void ProcedureNode::addStatement(Statement* s) {
	statements->push_back(s);
}

vector<Statement*>* ProcedureNode::getStatements() {
	return statements;
}

void ProcedureNode::setStatements(vector<Statement*>* some_statements) {
	statements = some_statements;
}

/**
 * find the index of an index variable by string
 *
 */
unsigned int ProcedureNode::assignLocalVariable(string s) {

	map<string, unsigned int>::iterator foundIter = local_variables->find(s);
	if (foundIter != local_variables->end()) {
		//
		// the variable already exists.
		//
		cout << "--- local variable " << s << " already exists with number "
				<< local_variables->at(s) << endl;
		return local_variables->at(s);
	} else {
		//
		// the variable does not exist. add it.
		//
		unsigned int newval = local_variables->size();
		local_variables->insert(pair<string, unsigned int>(s, newval));
		cout << "--- local variable " << s << " added with number " << newval
				<< endl;
		return newval;
	}
}

void ProcedureNode::analyze() {
	//
	// count the local variables
	//
	for (vector<Statement*>::iterator it = statements->begin();
			it != statements->end(); ++it) {
//		if ((*it)->isAssignment()) {
		// a_var = (AssignmentNode)(*it)->lhs;
		//	}
	}
}
