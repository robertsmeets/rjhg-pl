/*
 * ReturnNode.h
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#ifndef RETURNNODE_H_
#define RETURNNODE_H_

#include "Statement.h"
#include "ExpressionNode.h"
#include "CodeGenerator.h"

class ReturnNode:public Statement {
	ExpressionNode* return_expression;
public:
	ReturnNode(ExpressionNode*);
	virtual ~ReturnNode();
	virtual void emit(CodeGenerator*);
};

#endif /* RETURNNODE_H_ */
