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

using namespace std;

class CodeGenerator; // forward declaration

class AssignmentNode: public Statement {
	unsigned int lhs;
	ExpressionNode* rhs;
public:
	AssignmentNode(unsigned int, ExpressionNode*);
	virtual ~AssignmentNode();
	void setLhs(unsigned int);
	unsigned int getLhs();
	void setRhs(ExpressionNode*);
	ExpressionNode* getRhs();
	virtual void emit(CodeGenerator*);
};

#endif /* ASSIGNMENTNODE_H_ */
