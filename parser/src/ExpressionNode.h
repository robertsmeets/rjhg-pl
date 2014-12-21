/*
 * ExpressionNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef EXPRESSIONNODE_H_
#define EXPRESSIONNODE_H_

#include <vector>
#include <string>
#include "Node.h"
#include "ExpressionThing.h"

using namespace std;

class ExpressionNode: public Node {
	vector<ExpressionThing> rpn;

public:
	ExpressionNode();
	virtual ~ExpressionNode();
	void setRpn(vector<ExpressionThing>);
	vector<ExpressionThing> getRpn();
};

#endif /* EXPRESSIONNODE_H_ */
