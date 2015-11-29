/*
 * PrintNode.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: robert
 */

#include "PrintNode.h"

PrintNode::PrintNode(Expression* e)
{
	expression = e;
}

PrintNode::~PrintNode() {
}

void PrintNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	expression->emit(cg);
	//
	// return
	//
	cg->emit(9, 0, 0, this);
}

string PrintNode::stype() {
	return "print";
}

void PrintNode::print(int level) {
	for (int i=0;i<level;i++)
		{
			cout << "+" ;
		}
	cout << "PrintNode" << endl;
	// print_expression.print(level + 1);
}
