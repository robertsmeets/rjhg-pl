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

