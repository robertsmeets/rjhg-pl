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
private:
	string value;
public:
	VariableValue(string);
	virtual ~VariableValue();
	virtual void print(int);
	virtual void emit(CodeGenerator*);
	virtual string stype();
};

#endif /* VARIABLEVALUE_H_ */
