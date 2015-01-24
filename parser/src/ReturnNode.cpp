/*
 * ReturnNode.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#include "ReturnNode.h"

ReturnNode::ReturnNode(ProcedureNode* p,ExpressionNode* en) {
	//
	// it's valid to have an empty return node, in which case en = NULL
	//
	pn = p;
	return_expression = en;
}

ReturnNode::~ReturnNode() {
}

void ReturnNode::emit(CodeGenerator* cg) {
	//
	// remove all call info from the stack
	//
	int l = pn->getLocalVariables()->size();
	if (l > 0)
	{
		cg -> emit(10,0,l);
	}
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	if (return_expression != NULL)
	{
		cg->emitRpn(return_expression->getRpn(),pn);
	}
	//
	// return
	//
	cg->emit(2, 0, 0);
}

