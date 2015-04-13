/*
 * ExpressionNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef EXPRESSIONNODE_H_
#define EXPRESSIONNODE_H_

#include <iostream>
#include <vector>
#include <string>

#include "ExpressionThing.h"

using namespace std;

class ExpressionNode {
	vector<ExpressionThing> rpn;

public:
	ExpressionNode();
	virtual ~ExpressionNode();
	void setRpn(vector<ExpressionThing>);
	vector<ExpressionThing> getRpn();
	void print();
};

#endif /* EXPRESSIONNODE_H_ */
