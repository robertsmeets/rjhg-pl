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

void ExpressionNode::setRpn(vector<ExpressionThing> a_rpn)
{
    rpn = a_rpn;
}

vector<ExpressionThing> ExpressionNode::getRpn()
{
    return rpn;
}
