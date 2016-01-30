/*
 * PrintNode.h
 *
 *  Created on: Jan 24, 2015
 *      Author: robert
 */

#ifndef PRINTNODE_H_
#define PRINTNODE_H_

#include "Expression.h"
#include "CodeGenerator.h"
#include <iostream>

using namespace std;

class PrintNode : public Expression
{
private:
	ProcedureNode* pn;
	Expression* expression;
public:
	PrintNode(Expression*);
	virtual ~PrintNode();
	virtual void emit(CodeGenerator*,pProcedureNode*);
	virtual string stype();
	virtual void print(int);
};

#endif /* PRINTNODE_H_ */

