/*
 * ExpressionNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ExpressionNode.h"

ExpressionNode::ExpressionNode() {
}

ExpressionNode::~ExpressionNode() {
}

void ExpressionNode::setRpn(vector<ExpressionThing> a_rpn) {
	rpn = a_rpn;
}

vector<ExpressionThing> ExpressionNode::getRpn() {
	return rpn;
}

void ExpressionNode::print( int level) {
	for ( int i=0;i<level;i++)
	{
		cout << "+" ;
	}
	cout << "ExpressionNode" << endl;
	for (auto et : rpn) {
		et.print(level + 1);
	}
}
