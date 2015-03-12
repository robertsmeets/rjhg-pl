/*
 * ReturnNode.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: robert
 */

#include "ReturnNode.h"

ReturnNode::ReturnNode(ProcedureNode* p, ExpressionNode en,unsigned int linep,unsigned int charp,unsigned int absp) {
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
	//delete pn;
	//delete return_expression;
}

void ReturnNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	unsigned int l = 0;
	l = 1;
	cg->emitRpn(return_expression.getRpn(), pn);
	//
	// return
	//
	cg->emit(2, l, 0,this);
}

string ReturnNode::stype() {
	return "return";
}
