/*
 * WhileNode.h
 *
 *  Created on: Jan 21, 2015
 *      Author: robert
 */

#ifndef WHILENODE_H_
#define WHILENODE_H_

#include <vector>

#include "Statement.h"
#include "ExpressionNode.h"
#include "CodeGenerator.h"

class WhileNode: public Statement {
	vector<Statement*>* statements;
	ExpressionNode* expression;
public:
	WhileNode(ExpressionNode*, vector<Statement*>*);
	virtual ~WhileNode();
	void emit(CodeGenerator*);
};

#endif /* WHILENODE_H_ */
