/*
 * AssignmentNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include <iostream>
#include "AssignmentNode.h"

using namespace std;

AssignmentNode::AssignmentNode() {

}

AssignmentNode::~AssignmentNode() {

}

void AssignmentNode::setLhs(unsigned int a_lhs) {
	lhs = a_lhs;
}

void AssignmentNode::setRhs(ExpressionNode* a_rhs) {
	rhs = a_rhs;
}

unsigned int AssignmentNode::getLhs() {
	return lhs;
}
ExpressionNode* AssignmentNode::getRhs() {
	return rhs;
}

void AssignmentNode::emit(CodeGenerator* cg)
{
	//
	// emit the calculation instructions
	//
	cg -> emitRpn(rhs->getRpn());
	//
	// emit a "sto" to store the value in a variable
	//
	cg -> emit(4, 0, lhs);

}
