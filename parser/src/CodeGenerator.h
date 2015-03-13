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
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#include "ProgramNode.h"
#include "ProcedureNode.h"
#include "ExpressionThing.h"
#include "AssignmentNode.h"
#include "PException.h"
#include "DebugInfo.h"

using namespace std;

class ProgramNode;
// forward declaration

class ProcedureNode;
// forward declaration

class CodeGenerator {
	ofstream myfile;
	map<string, ProcedureNode*> procaddresses;
	map<unsigned int, string> callpoints;
	char* codebuffer;
	unsigned int here;
	unsigned int codesize;
	ProgramNode* pn;
	DebugInfo* di;
	unordered_map<string, unsigned int> opr_mapping;
public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(ProgramNode*,DebugInfo*);
	void start_proc(ProcedureNode*);
	void emit(char, unsigned short int, unsigned short int,Statement*);
	void emitRpn(vector<ExpressionThing>, ProcedureNode*,Statement*);
	void emitOperation(string,Statement*);
	void fix_proc_addresses();
	void addCallAddress(unsigned int, string);
	unsigned int getHere();
	void emit_to_file();
	void fix(unsigned int, unsigned int);
	char* getCodeBuffer();
	void addCallTo(string,Statement*);
	Statement* procDefined(string);
};

#endif /* CODEGENERATOR_H_ */

