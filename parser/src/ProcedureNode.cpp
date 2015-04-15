/*
 * ProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProcedureNode.h"

using namespace std;

/**
 * constructor
 */
ProcedureNode::ProcedureNode(ClassDefinition* a_cd, string method_name,
		ProgramNode* a_pn) {
	pn = a_pn;
	cd = a_cd;
	name = method_name;
	if (cd != NULL) {
		method_number = pn->assignMethodNumber(method_name);
	}
	parameters = new vector<string>();
	local_variables = new map<string, unsigned int>;
	proc_address = 0;

}

/**
 * destructor
 */
ProcedureNode::~ProcedureNode() {
	//
	// delete all the statements
	//
	parameters->erase(parameters->begin(), parameters->end());
	delete parameters;
	delete local_variables;
	for_each(statements.begin(), statements.end(),
			delete_pointed_to<Statement>);
	statements.clear();
}

/**
 * setter methods
 */
void ProcedureNode::setName(string a_name) {
	name = a_name;
}

void ProcedureNode::setProcAddress(unsigned int a) {
	proc_address = a;
}

void ProcedureNode::addParameter(string a_parameter) {
	parameters->push_back(a_parameter);
}

void ProcedureNode::setStatements(vector<Statement*> some_statements) {
	statements = some_statements;
}

/**
 * getter methods
 */
string ProcedureNode::getName() {
	return name;
}

unsigned int ProcedureNode::getProcAddress() {
	return proc_address;
}

vector<string>* ProcedureNode::getParameters() {
	return parameters;
}

map<string, unsigned int>* ProcedureNode::getLocalVariables() {
	return local_variables;
}

vector<Statement*> ProcedureNode::getStatements() {
	return statements;
}

unsigned int ProcedureNode::getMethodNumber() {
	return method_number;
}

string ProcedureNode::getFullMethodName() {
	return cd->getName() + "." + name;
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
		return local_variables->at(s);
	} else {
		//
		// the variable does not exist. add it.
		//
		unsigned int newval = local_variables->size();
		local_variables->insert(pair<string, unsigned int>(s, newval));
		return newval;
	}
}

/**
 * add a return statement if the procedure does not define one
 */
void ProcedureNode::fixReturn() {
	//
	// if return is missing, add it
	//
	unsigned int sz = statements.size();
	bool addreturn = false;
	unsigned int linepos = 0;
	unsigned int charpos = 0;
	unsigned int abspos = 0;
	if ((sz == 0)) {
		addreturn = true;
	} else {
		Statement* last = statements.at(sz - 1);
		linepos = last->linepos;
		charpos = last->charpos;
		abspos = last->abspos;
		addreturn = (last->stype() != "return");
	}
	if (addreturn) {
		ExpressionNode en;
		statements.push_back(
				new ReturnNode(this, en, linepos, charpos, abspos));
	}
}

void ProcedureNode::print() {
	cout << "ProcedureNode " << name << "(" << parameters->size() << " params)"
			<< endl;
	for (auto s : statements) {
		s->print();
	}
}

unsigned int ProcedureNode::getInstanceVarNum(string name) {
	if (cd == NULL) {
		throw PException("Variable " + name + " not found");
	}
	unsigned int i = 0;
	for (auto it : cd->getInstanceVariables()) {
		if (it == name) {
			return i;
		}
	}
	throw PException("instance var <" + name + "> not found");
}
