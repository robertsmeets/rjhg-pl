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
#include <iostream>

#include "Statement.h"
#include "ExpressionNode.h"
#include "CodeGenerator.h"

using namespace std;

class ProcedureCallNode: public Statement {
	ProcedureNode* pn;
	string procedure_name;
	vector<ExpressionNode> parameter_exps;
	ExpressionNode LhsExpression;
	bool method;
public:
	ProcedureCallNode(ProcedureNode*,unsigned int,unsigned int,unsigned int);
	virtual ~ProcedureCallNode();
	void setProcedureName(string);
	string getProcedureName();
	virtual void emit(CodeGenerator*);
	void print(int);
	void addParametersExpression(ExpressionNode);
	void setLhsExpression(ExpressionNode);
	void setMethod(bool);
	virtual string stype();
};

#endif /* PROCEDURECALLNODE_H_ */
