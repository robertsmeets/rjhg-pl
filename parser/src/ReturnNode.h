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
#include "ProcedureNode.h"


using namespace std;

class ProcedureNode;
// forward declaration

class ReturnNode: public Statement {
	ProcedureNode* pn;
	ExpressionNode return_expression;
public:
	ReturnNode(ProcedureNode*, ExpressionNode, unsigned int, unsigned int,
			unsigned int);

	virtual ~ReturnNode();
	virtual void emit(CodeGenerator*);
	void print();
	virtual string stype();
};

#endif /* RETURNNODE_H_ */
