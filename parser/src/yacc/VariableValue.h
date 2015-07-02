/*
 * VariableValue.h
 *
 *  Created on: Jul 1, 2015
 *      Author: Robert
 */

#ifndef VARIABLEVALUE_H_
#define VARIABLEVALUE_H_

#include "Expression.h"

#include <iostream>

using namespace std;

class VariableValue : public Expression {
public:
	VariableValue(string);
	virtual ~VariableValue();
};

#endif /* VARIABLEVALUE_H_ */
