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

#include "ClassDefinition.h"

#include "DebugInfo.h"
#include "Expression.h"
#include "ProgramNode.h"
#include "Statements.h"

using namespace std;

class ProgramNode; // forward declaration

class ProcedureNode; // forward declaration

class ClassDefinition; // forward declaration

class Expression; // forward declaration

class CodeGenerator {
  bool debug;
  ofstream myfile;
  map<string, ProcedureNode*> procaddresses;
  map<uint16_t, string> callpoints;
  char* codebuffer;
  uint16_t here;
  uint16_t codesize;
  ProgramNode* pn;
  DebugInfo* di;
  unordered_map<char, unsigned int> opr_mapping;
public:
  CodeGenerator();
  virtual ~CodeGenerator();
  char* getCodeBuffer();
  uint16_t getHere();
  ProgramNode* getProgramNode();
  void start(ProgramNode*, DebugInfo*, bool);
  void start_proc(ProcedureNode*);
  void emit(char, unsigned short int, unsigned short int, Expression*);
  void emitOperation(char, Expression*);
  void fix_proc_addresses();
  void addCallAddress(uint16_t, string);
  void emit_to_file();
  void fix(uint16_t, uint16_t);
  void addCallTo(string, Expression*);
  void addCallToClassConstructor(ClassDefinition*, Expression*);
  void addCallToMethod(string, Expression*);
  void addCallToProc(string, Expression*);
  void addCallToProcedure(string, Expression*);
  Expression* procDefined(string);
  void emitByte(char);
  void emit2Byte(uint16_t);
  char* allot(int size);
  ClassDefinition* getClassDefinition(string);
};

#endif /* CODEGENERATOR_H_ */

