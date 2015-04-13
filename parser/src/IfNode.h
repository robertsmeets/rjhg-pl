/*
 * IfNode.h
 *
 *  Created on: Jan 16, 2015
 *      Author: robert
 */

#ifndef IFNODE_H_
#define IFNODE_H_

#include <vector>

#include "Statement.h"
#include "ExpressionNode.h"
#include "CodeGenerator.h"

class IfNode:public Statement {
	ProcedureNode* pn;
	vector<Statement*> statements_true;
	vector<Statement*> statements_false;
	ExpressionNode if_expression;
public:
	IfNode(ProcedureNode*,ExpressionNode,vector<Statement*>,vector<Statement*>,unsigned int,unsigned int,unsigned int);
	virtual ~IfNode();
	void emit(CodeGenerator*);
	virtual string stype();
	void print();
};

#endif /* IFNODE_H_ */

