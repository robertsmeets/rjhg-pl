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
#include "CFunctions.h"

using namespace std;

class WhileNode: public Statement {
	ProcedureNode* pn;
	vector<Statement*> statements;
	ExpressionNode expression;
public:
	WhileNode(ProcedureNode*,ExpressionNode, vector<Statement*>,unsigned int,unsigned int,unsigned int);
	virtual ~WhileNode();
	void emit(CodeGenerator*);
	void print(unsigned int);
	virtual string stype();
};

#endif /* WHILENODE_H_ */
