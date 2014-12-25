/*
 * ProgramNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef PROGRAMNODE_H_
#define PROGRAMNODE_H_

#include <vector>

#include "ProcedureNode.h"

using namespace std;

class ProgramNode{
public:
	ProgramNode();
	virtual ~ProgramNode();
	vector<ProcedureNode> procedures;
	void addProcedure(ProcedureNode);
	vector<ProcedureNode> getProcedures();
};

#endif /* PROGRAMNODE_H_ */
