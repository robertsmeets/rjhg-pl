/*
 * AssignmentNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef ASSIGNMENTNODE_H_
#define ASSIGNMENTNODE_H_

#include <iostream>

#include "Statement.h"
#include "ExpressionNode.h"
#include "CodeGenerator.h"
#include "ProcedureNode.h"

using namespace std;

class CodeGenerator; // forward declaration

// class Statement ; // forward declaration

class ProcedureNode;

class AssignmentNode: public Statement {
	ProcedureNode* pn;
	unsigned int lhs;
	ExpressionNode rhs;
public:
	AssignmentNode(ProcedureNode*,unsigned int, ExpressionNode,unsigned int, unsigned int,unsigned int);
	virtual ~AssignmentNode();
	void setLhs(unsigned int);
	unsigned int getLhs();
	void setRhs(ExpressionNode);
	ExpressionNode getRhs();
	virtual void emit(CodeGenerator*);
	virtual string stype();
};

#endif /* ASSIGNMENTNODE_H_ */
