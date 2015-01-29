/*
 * PrintStatement.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: robert
 */

#include "PrintNode.h"

PrintNode::PrintNode(ProcedureNode* p,ExpressionNode* en) {
	pn = p;
	print_expression = en;
}

PrintNode::~PrintNode() {

}

void PrintNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	if (print_expression != NULL)
	{
		cg->emitRpn(print_expression->getRpn(),pn);
	}
	//
	// return
	//
	cg->emit(9, 0, 0);
}
string PrintNode::stype()
{
	 return "print";
}
