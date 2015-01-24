/*
 * Statement.h
 *
 *  Created on: Jan 9, 2015
 *      Author: robert
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <iostream>

// #include "ProcedureNode.h"

// #include "CodeGenerator.h"
class CodeGenerator; // forward declaration

// class ProcedureNode; // forward declaration

class Statement {

public:
	Statement();
	virtual ~Statement();
	virtual void emit(CodeGenerator*) = 0;
};

#endif /* STATEMENT_H_ */
