/*
 * VariableValue.h
 *
 *  Created on: Jul 1, 2015
 *      Author: Robert
 */

#ifndef VARIABLEVALUE_H_
#define VARIABLEVALUE_H_

#include "Expression.h"
#include "CodeGenerator.h"

#include "stdio.h"

using namespace std;

class VariableValue : public Expression {
private:
	string value;
public:
	VariableValue(string);
	virtual ~VariableValue();
	virtual void print(int);
	virtual void emit(CodeGenerator*,ProcedureNode*);
	virtual string stype();
	string getName();
};

#endif /* VARIABLEVALUE_H_ */
