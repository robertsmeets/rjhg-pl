/*
 * AssignmentNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef ASSIGNMENTNODE_H_
#define ASSIGNMENTNODE_H_

#include "Node.h"
#include "ExpressionNode.h"

using namespace std;

class AssignmentNode: public Node {
	unsigned int lhs;
	ExpressionNode rhs;
public:
	AssignmentNode();
	virtual ~AssignmentNode();
	void setLhs(unsigned int);
	unsigned int getLhs();
	void setRhs(ExpressionNode);
	ExpressionNode getRhs();
};

#endif /* ASSIGNMENTNODE_H_ */
