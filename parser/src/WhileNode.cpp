/*
 * WhileNode.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robert
 */

#include "WhileNode.h"

WhileNode::WhileNode(ProcedureNode* p, ExpressionNode en, vector<Statement*> s,
		unsigned int linep, unsigned int charp,unsigned int absp) {
	linepos = linep;
	charpos = charp;
	abspos = absp;
	pn = p;
	expression = en;
	statements = s;
}

WhileNode::~WhileNode() {
	for_each(statements.begin(), statements.end(),
			delete_pointed_to<Statement>);
	statements.clear();
}

void WhileNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	unsigned int dest_address1 = cg->getHere();
	cg->emitRpn(expression.getRpn(), pn,this);
	//
	// emit JPC, jump if the stack top is 0 (false)
	//
	cg->emit(8, 0, 0, this);
	unsigned int jump_address2 = cg->getHere() - 2;
	//
	// emit the block statements
	//
	for (vector<Statement*>::iterator it = statements.begin();
			it != statements.end(); ++it) {
		(*it)->emit(cg);
	}
	//
	// jump back to the dest_address
	//
	cg->emit(7, 0, 0, this);
	unsigned int dest_address2 = cg->getHere();
	unsigned int jump_address1 = cg->getHere() - 2;
	cg->fix(jump_address1, dest_address1);
	cg->fix(jump_address2, dest_address2);

}

string WhileNode::stype() {
	return "while";
}

void WhileNode::print(unsigned int level)
{
	for (unsigned int i=0;i<level;i++)
		{
			cout << "+" ;
		}

	cout << "WhileNode" << endl;
}
