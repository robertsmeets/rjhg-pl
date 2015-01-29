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

class ProcedureNode; // forward declaration

class ProgramNode{
	vector<ProcedureNode*> procedures;
public:
	ProgramNode();
	virtual ~ProgramNode();
	void addProcedure(ProcedureNode*);
	vector<ProcedureNode*> getProcedures();

};

#endif /* PROGRAMNODE_H_ */
