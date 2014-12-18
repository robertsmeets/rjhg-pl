/*
 * AssignmentNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "AssignmentNode.h"

AssignmentNode::AssignmentNode() {


}

AssignmentNode::~AssignmentNode() {

}

void AssignmentNode::setLhs(string a_lhs)
{
   lhs = a_lhs;
}

void AssignmentNode::setRhs(ExpressionNode a_rhs)
{
   rhs = a_rhs;
}
