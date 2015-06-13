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
#include "ClassDefinition.h"

using namespace std;

class ProgramNode;
// forward declaration

class ProcedureNode;
// forward declaration

class CodeGenerator {
	ofstream myfile;
	map<string, ProcedureNode*> procaddresses;
	map<uint16_t, string> callpoints;
	char* codebuffer;
	uint16_t here;
	uint16_t codesize;
	ProgramNode* pn;
	DebugInfo* di;
	unordered_map<string, unsigned int> opr_mapping;
public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(ProgramNode*, DebugInfo*);
	void start_proc(ProcedureNode*);
	void emit(char, unsigned short int, unsigned short int, Statement*);
	void emitRpn(vector<ExpressionThing>, ProcedureNode*, Statement*);
	void emitOperation(string, Statement*);
	void fix_proc_addresses();
	void addCallAddress(uint16_t, string);
	uint16_t getHere();
	void emit_to_file();
	void fix(uint16_t, uint16_t);
	char* getCodeBuffer();
	void addCallTo(string, Statement*);
	void addCallToClassConstructor(ClassDefinition*, Statement*);
	void addCallToMethod(string, Statement*);
	void addCallToProc(string, Statement*);
	void addCallToProcedure(string, Statement*);
	Statement* procDefined(string);
	void emitByte(char);
	void emit2Byte(uint16_t);
};

#endif /* CODEGENERATOR_H_ */

