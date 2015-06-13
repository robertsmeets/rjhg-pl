/*
 * PrintStatement.h
 *
 *  Created on: Jan 24, 2015
 *      Author: robert
 */

#ifndef PRINTNODE_H_
#define PRINTNODE_H_

#include "ExpressionNode.h"
#include "CodeGenerator.h"
#include "ExpressionNode.h"


class PrintNode : public Statement{
	ProcedureNode* pn;
	ExpressionNode print_expression;
public:
	PrintNode(ProcedureNode*,ExpressionNode,unsigned int,unsigned int,unsigned int);
	virtual ~PrintNode();
	virtual void emit(CodeGenerator*);
	virtual string stype();
	void print(unsigned int);
};

#endif /* PRINTNODE_H_ */

