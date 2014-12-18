/*
 * AssignmentNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef ASSIGNMENTNODE_H_
#define ASSIGNMENTNODE_H_

#include <string>
#include "Node.h"
#include "ExpressionNode.h"

using namespace std;

class AssignmentNode: public Node {
	string lhs;
	ExpressionNode rhs;
public:
	AssignmentNode();
	virtual ~AssignmentNode();
	void setLhs(string);
	void setRhs(ExpressionNode);


};

#endif /* ASSIGNMENTNODE_H_ */
