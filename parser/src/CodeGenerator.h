/*
 * CodeGenerator.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <fstream>
#include <map>
#include "ProgramNode.h"
#include "ProcedureNode.h"
#include "ExpressionThing.h"

using namespace std;

class ProgramNode ; // forward declaration

class ProcedureNode ; // forward declaration

class CodeGenerator {
	ofstream myfile;
	map<string, unsigned int> procadresses;
	map<unsigned int,string> callpoints;
	vector<char> codebuffer;
	unsigned int here;


public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(ProgramNode);
	void start_proc(ProcedureNode*);
	void emit(char, unsigned short int, unsigned short int);
	void emitRpn(vector<ExpressionThing>);
	void emitOperation(string avalue);
	void fix_proc_addresses();
	void addCallAddress(unsigned int,string);
	unsigned int getHere();
	void emit_to_file();
	void printcodebuffer();
};

#endif /* CODEGENERATOR_H_ */


