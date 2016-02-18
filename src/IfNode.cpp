/*
 * IfNode.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: robert
 */

#include "IfNode.h"

IfNode::IfNode(ProcedureNode* p, ExpressionNode en, vector<Statement*> s_true,
		vector<Statement*> s_false, unsigned int linep, unsigned int charp,unsigned int absp) {
	linepos = linep;
	charpos = charp;
	abspos =absp;
	pn = p;
	if_expression = en;
	statements_true = s_true;
	statements_false = s_false;

}

IfNode::~IfNode() {

}

void IfNode::emit(CodeGenerator* cg) {
	//
	// emit the instructions to calculate the value and put it on the stack
	//
	cg->emitRpn(if_expression.getRpn(), pn,this);
	if (statements_false.empty()) {
		//
		// emit JPC, jump if the stack top is 0 (false)
		//
		cg->emit(8, 0, 0, this);
		unsigned int jump_address = cg->getHere() - 2;
		for (vector<Statement*>::iterator it = statements_true.begin();
				it != statements_true.end(); ++it) {
			(*it)->emit(cg);
		}
		unsigned int dest_address = cg->getHere();
		cg->fix(jump_address, dest_address);
	} else {
		cg->emit(8, 0, 0, this);
		unsigned int jump_address1 = cg->getHere() - 2;
		for (vector<Statement*>::iterator it = statements_false.begin();
				it != statements_false.end(); ++it) {
			(*it)->emit(cg);
		}
		//
		// jump
		//
		cg->emit(7, 0, 0, this);
		unsigned int jump_address2 = cg->getHere() - 2;
		unsigned int dest_address1 = cg->getHere();
		for (vector<Statement*>::iterator it = statements_true.begin();
				it != statements_true.end(); ++it) {
			(*it)->emit(cg);
		}
		unsigned int dest_address2 = cg->getHere();
		cg->fix(jump_address1, dest_address1);
		cg->fix(jump_address2, dest_address2);
	}
}

string IfNode::stype() {
	return "if";
}

void IfNode::print(int level)
{for ( int i=0;i<level;i++)
{
	printf("+" ;
}

	printf("IfNode" );
}


