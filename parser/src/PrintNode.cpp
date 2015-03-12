/*
 * PrintStatement.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: robert
 */

#include "PrintNode.h"

PrintNode::PrintNode(ProcedureNode* p, ExpressionNode en, unsigned int linep,
		unsigned int charp,unsigned int absp) {
	linepos = linep;
	charpos = charp;
	abspos =absp;
	pn = p;
	print_expression = en;
}

PrintNode::~PrintNode() {
}

void PrintNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	cg->emitRpn(print_expression.getRpn(), pn,this);
	//
	// return
	//
	cg->emit(9, 0, 0,this);
}
string PrintNode::stype() {
	return "print";
}
