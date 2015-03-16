/*
 * ReturnNode.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#include "ReturnNode.h"

ReturnNode::ReturnNode(ProcedureNode* p, ExpressionNode en, unsigned int linep,
		unsigned int charp, unsigned int absp) {
	//
	// it's valid to have an empty return node, in which case en = NULL
	//
	linepos = linep;
	charpos = charp;
	abspos = absp;
	pn = p;
	return_expression = en;
}

ReturnNode::~ReturnNode() {
}

void ReturnNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	unsigned int l;
	vector<ExpressionThing> rpn = return_expression.getRpn();
	if (rpn.empty())
	{
		l=0;
	}
	else
	{
		l=1;
		cg->emitRpn(rpn, pn, this);
	}
	//
	// return
	//
	cg->emit(2, l, 0, this);
}

string ReturnNode::stype() {
	return "return";
}
