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
#include "stdio.h"
#include <string>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#include "pProgramNode.h"
#include "ExpressionThing.h"

#include "DebugInfo.h"
#include "pClassDefinition.h"
#include "Expression.h"
#include "Statements.h"

using namespace std;

class pProgramNode; // forward declaration

class pProcedureNode; // forward declaration

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
  unordered_map<char, unsigned int> opr_mapping;
public:
  CodeGenerator();
  virtual ~CodeGenerator();
  char* getCodeBuffer();
  uint16_t getHere();
  pProgramNode* getProgramNode();
  void start(pProgramNode*, DebugInfo*);
  void start_proc(pProcedureNode*);
  void emit(char, unsigned short int, unsigned short int, Expression*);
  void emitRpn(vector<ExpressionThing>, pProcedureNode*, Expression*);
  void emitOperation(char, Expression*);
  void fix_proc_addresses();
  void addCallAddress(uint16_t, string);
  void emit_to_file();
  void fix(uint16_t, uint16_t);
  void addCallTo(string, Expression*);
  void addCallToClassConstructor(pClassDefinition*, Expression*);
  void addCallToMethod(string, Expression*);
  void addCallToProc(string, Expression*);
  void addCallToProcedure(string, Expression*);
  Expression* procDefined(string);
  void emitByte(char);
  void emit2Byte(uint16_t);
  char* allot(int size);
  pClassDefinition* getClassDefinition(string);
};

#endif /* CODEGENERATOR_H_ */
