#include "pProcedureNode.h"

#include <iostream>

 /*
 * pProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */


using namespace std;


/**
 * constructor
 */
pProcedureNode::pProcedureNode(string a_name,Statements* some_statements)
{
   name = a_name;
	 statements = some_statements;
}


/**
 * destructor
 */
pProcedureNode::~pProcedureNode() {
	//
	// delete all the statements
	//
	/* parameters->erase(parameters->begin(), parameters->end());
	delete parameters;
	delete local_variables;
	for_each(statements.begin(), statements.end(),
			delete_pointed_to<Statement>);
	statements.clear(); */
}

/**
 * getter methods
 */

 string pProcedureNode::getName()
 {
    return name;
 }
 

/**
 * setter methods
 */
void pProcedureNode::setName(string a_name) {
	name = a_name;
}

void pProcedureNode::setProcAddress(uint16_t a) {
	proc_address = a;
}

void pProcedureNode::addParameter(string a_parameter) {
	parameters->push_back(a_parameter);
}

void pProcedureNode::setStatements(vector<Statement*> some_statements) {
	// statements = some_statements;
}


uint16_t pProcedureNode::getProcAddress() {
	return proc_address;
}

vector<string>* pProcedureNode::getParameters() {
	return parameters;
}

map<string, uint16_t>* pProcedureNode::getLocalVariables() {
	return local_variables;
}


/**
 * find the index of an index variable by string
 *
 */
uint16_t pProcedureNode::assignLocalVariable(string s) {

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
void pProcedureNode::fixReturn() {
	//
	// if return is missing, add it
	//
	/*uint16_t sz = statements.size();
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
	} */
}

void pProcedureNode::print(unsigned int level) {
	for (unsigned int i=0;i<level;i++)
	{
		cout << "+" ;
	}
	cout << "pProcedureNode " << name << "(" << parameters->size() << " params)"
			<< endl;
	/* for (auto s : statements) {
		s->print(level + 1);
	} */
}

int pProcedureNode::getInstanceVarNum(string name) {
	/* if (cd == NULL) {
		return -1;
	}
	int i = 0;
	for (auto it : cd->getInstanceVariables()) {
		if (it == name) {
			return i;
		}
		i++;
	}
	return -1; */
}

Statements* pProcedureNode::getStatements()
{
	return statements;
}

