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

class IfNode:public Statement {
	vector<Statement*>* statements_true;
	vector<Statement*>* statements_false;
	ExpressionNode* if_expression;
public:
	IfNode(ExpressionNode*,vector<Statement*>*,vector<Statement*>*);
	virtual ~IfNode();
	void emit(CodeGenerator*);
};

#endif /* IFNODE_H_ */

