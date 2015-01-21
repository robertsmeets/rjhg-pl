/*
 * WhileNode.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robert
 */

#include "WhileNode.h"

WhileNode::WhileNode(ExpressionNode* en, vector<Statement*>* s) {
	expression = en;
	statements = s;
}

WhileNode::~WhileNode() {
}

void WhileNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	unsigned int dest_address = cg->getHere();
	cg->emitRpn(expression->getRpn());
	for (vector<Statement*>::iterator it = statements->begin();
			it != statements->end(); ++it) {
		(*it)->emit(cg);
	}
	//
	// emit JPC, jump if the stack top is 0 (false)
	//
	cg->emit(8, 0, 0);
	unsigned int jump_address = cg->getHere() - 2;
	cg->fix(jump_address, dest_address);
}
