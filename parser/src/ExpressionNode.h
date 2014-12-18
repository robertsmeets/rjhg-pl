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

using namespace std;

class ExpressionNode: public Node {
	vector<string> rpn;

public:
	ExpressionNode();
	virtual ~ExpressionNode();
	void setRpn(vector<string>);
};

#endif /* EXPRESSIONNODE_H_ */
