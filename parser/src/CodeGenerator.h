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

#include "pProgramNode.h"
#include "ProcedureNode.h"
#include "ExpressionThing.h"
#include "AssignmentNode.h"
#include "PException.h"
#include "DebugInfo.h"
#include "pClassDefinition.h"
#include "pMethodDefinition.h"
#include "Expression.h"
#include "Statements.h"

using namespace std;

class pProgramNode; // forward declaration

class pProcedureNode; // forward declaration

class pMethodDefinition; // forward declaration

class pClassDefinition; // forward declaration

class Expression; // forward declaration

class CodeGenerator {
	ofstream myfile;
	map<string, pProcedureNode*> procaddresses;
	map<uint16_t, string> callpoints;
	char* codebuffer;
	uint16_t here;
	uint16_t codesize;
	pProgramNode* pn;
	DebugInfo* di;
	unordered_map<string, unsigned int> opr_mapping;
public:
	CodeGenerator();
	virtual ~CodeGenerator();
	void start(pProgramNode*, DebugInfo*);
	void start_proc(pProcedureNode*);
	void start_method(pMethodDefinition*);
	void emit(char, unsigned short int, unsigned short int, Expression*);
	void emitRpn(vector<ExpressionThing>, pProcedureNode*, Expression*);
	void emitOperation(string, Expression*);
	void fix_proc_addresses();
	void addCallAddress(uint16_t, string);
	uint16_t getHere();
	void emit_to_file();
	void fix(uint16_t, uint16_t);
	char* getCodeBuffer();
	void addCallTo(string, Expression*);
	void addCallToClassConstructor(pClassDefinition*, Expression*);
	void addCallToMethod(string, Expression*);
	void addCallToProc(string, Expression*);
	void addCallToProcedure(string, Expression*);
	Expression* procDefined(string);
	void emitByte(char);
	void emit2Byte(uint16_t);
};

#endif /* CODEGENERATOR_H_ */

