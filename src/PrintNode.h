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
#include "stdio.h"

using namespace std;

class PrintNode : public Expression
{
private:
   Expression* expression;
public:
   PrintNode(Expression*);
   virtual ~PrintNode();
   virtual void emit(CodeGenerator*,ProcedureNode*);
   virtual string stype();
   virtual void print(int);
   virtual void setTopLevel();
};

#endif /* PRINTNODE_H_ */

