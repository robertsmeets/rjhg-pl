/*
 * CodeGenerator.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ProgramNode.h"

class CodeGenerator {
public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(ProgramNode);
	void start_proc(ProcedureNode);

};

#endif /* CODEGENERATOR_H_ */
