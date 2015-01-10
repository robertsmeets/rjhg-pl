/*
 * ProcedureCallNode.h
 *
 *  Created on: Jan 8, 2015
 *      Author: robert
 */

#ifndef PROCEDURECALLNODE_H_
#define PROCEDURECALLNODE_H_

#include <string>
#include "Statement.h"

using namespace std;

class ProcedureCallNode:public Statement {
	string procedure_name;
public:
	ProcedureCallNode();
	virtual ~ProcedureCallNode();
	void setProcedureName(string);
	string getProcedureName();
	virtual void emit(CodeGenerator*);
};

#endif /* PROCEDURECALLNODE_H_ */
