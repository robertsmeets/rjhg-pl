/*
 * CodeGenerator.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <fstream>
#include "ProgramNode.h"
#include "ExpressionThing.h"

using namespace std;

class CodeGenerator {
	ofstream myfile;


public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(ProgramNode);
	void start_proc(ProcedureNode);
	void emit(char, unsigned short int, unsigned short int);
	void emitRpn(vector<ExpressionThing>);
	void emitOperation(string avalue);

};

#endif /* CODEGENERATOR_H_ */


