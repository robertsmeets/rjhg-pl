/*
 * ProgramNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProgramNode.h"

ProgramNode::ProgramNode() {
	procedures = vector<ProcedureNode*>();
}

ProgramNode::~ProgramNode() {

}

void ProgramNode::addProcedure(ProcedureNode* pn) {
	procedures.push_back(pn);
}

vector<ProcedureNode*> ProgramNode::getProcedures() {
	return procedures;
}

unsigned int ProgramNode::getProcedure(string pname) {
	unsigned int i = 0;
	for (vector<ProcedureNode*>::iterator it = procedures.begin();
			it != procedures.end(); ++it) {
		ProcedureNode* a_proc = *it;
		string pname = a_proc->getName();
		if (pname == pname) {
			return i;
		}
		i++;
	}
	return 0;
}
