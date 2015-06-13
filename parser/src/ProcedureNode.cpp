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
	local_variables = new map<string, uint16_t>;
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

void ProcedureNode::setProcAddress(uint16_t a) {
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

uint16_t ProcedureNode::getProcAddress() {
	return proc_address;
}

vector<string>* ProcedureNode::getParameters() {
	return parameters;
}

map<string, uint16_t>* ProcedureNode::getLocalVariables() {
	return local_variables;
}

vector<Statement*> ProcedureNode::getStatements() {
	return statements;
}

uint16_t ProcedureNode::getMethodNumber() {
	return method_number;
}

string ProcedureNode::getFullMethodName() {
	return cd->getName() + "." + name;
}

/**
 * find the index of an index variable by string
 *
 */
uint16_t ProcedureNode::assignLocalVariable(string s) {

	map<string, uint16_t>::iterator foundIter = local_variables->find(s);
	if (foundIter != local_variables->end()) {
		//
		// the variable already exists.
		//
		return local_variables->at(s);
	} else {
		//
		// the variable does not exist. add it.
		//
		uint16_t newval = local_variables->size();
		local_variables->insert(pair<string, uint16_t>(s, newval));
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
	uint16_t sz = statements.size();
	bool addreturn = false;
	uint16_t linepos = 0;
	uint16_t charpos = 0;
	uint16_t abspos = 0;
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

void ProcedureNode::print(unsigned int level) {
	for (unsigned int i=0;i<level;i++)
	{
		cout << "+" ;
	}
	cout << "ProcedureNode " << name << "(" << parameters->size() << " params)"
			<< endl;
	for (auto s : statements) {
		s->print(level + 1);
	}
}

int ProcedureNode::getInstanceVarNum(string name) {
	if (cd == NULL) {
		return -1;
	}
	int i = 0;
	for (auto it : cd->getInstanceVariables()) {
		if (it == name) {
			return i;
		}
		i++;
	}
	return -1;
}
