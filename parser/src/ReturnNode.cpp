/*
 * ReturnNode.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#include "ReturnNode.h"


ReturnNode::ReturnNode(ExpressionNode* en) {
	//
	// it's valid to have an empty return node, in which case en = NULL
	//
	return_expression = en;
}

ReturnNode::~ReturnNode() {
}

void ReturnNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	if (return_expression != NULL)
	{
		cg->emitRpn(return_expression->getRpn());
	}
	//
	// return
	//
	cg->emit(2, 0, 0);
}
