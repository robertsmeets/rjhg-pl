/*
 * AssignmentNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "AssignmentNode.h"

using namespace std;

AssignmentNode::AssignmentNode(ProcedureNode* p, unsigned int an_lhs,
		ExpressionNode a_rhs, unsigned int linep, unsigned int charp,
		unsigned int absp) {
	pn = p;
	lhs = an_lhs;
	rhs = a_rhs;
	linepos = linep;
	charpos = charp;
	abspos = absp;
}

AssignmentNode::~AssignmentNode() {
}

void AssignmentNode::setLhs(unsigned int a_lhs) {
	lhs = a_lhs;
}

void AssignmentNode::setRhs(ExpressionNode a_rhs) {
	rhs = a_rhs;
}

unsigned int AssignmentNode::getLhs() {
	return lhs;
}
ExpressionNode AssignmentNode::getRhs() {
	return rhs;
}

void AssignmentNode::emit(CodeGenerator* cg) {
	//
	// emit the calculation instructions
	//
	cg->emitRpn(rhs.getRpn(), pn, this);
	//
	// emit a "sto" to store the value in a variable
	//
	// stack order:
	// parameters
	// local variables
	// therefore if we want a local variable, we need to add the amount of parameters to it
	//
	cg->emit(4, 0, lhs + pn->getParameters()->size(), this);
}

string AssignmentNode::stype() {
	return "assignment";
}

void AssignmentNode::print()
{
	cout << "AssignmentNode" ;
	rhs.print();
	cout << endl;
}
