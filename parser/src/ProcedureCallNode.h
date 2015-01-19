/*
 * ProcedureCallNode.h
 *
 *  Created on: Jan 8, 2015
 *      Author: robert
 */

#ifndef PROCEDURECALLNODE_H_
#define PROCEDURECALLNODE_H_

#include <vector>
#include <string>
#include "Statement.h"
#include "ExpressionNode.h"

using namespace std;

class ProcedureCallNode:public Statement {
	string procedure_name;
	vector<ExpressionNode*> parameter_exps;
public:
	ProcedureCallNode();
	virtual ~ProcedureCallNode();
	void setProcedureName(string);
	string getProcedureName();
	virtual void emit(CodeGenerator*);
	void addParametersExpression(ExpressionNode*);
};

#endif /* PROCEDURECALLNODE_H_ */
