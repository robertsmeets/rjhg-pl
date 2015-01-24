/*
 * ReturnNode.h
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#ifndef RETURNNODE_H_
#define RETURNNODE_H_

#include "ExpressionNode.h"
#include "CodeGenerator.h"
#include "ExpressionNode.h"

class ReturnNode:public Statement {
	ProcedureNode* pn;
	ExpressionNode* return_expression;
public:
	ReturnNode(ProcedureNode*,ExpressionNode*);
	virtual ~ReturnNode();
	virtual void emit(CodeGenerator*);
};

#endif /* RETURNNODE_H_ */
