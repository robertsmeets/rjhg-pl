/*
 * ProgramNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */
#ifndef PROGRAMNODE_H_
#define PROGRAMNODE_H_

#include "Extern.h"

#include <string>
#include <vector>
#include "ClassDefinition.h"
#include "ProcedureNode.h"

using namespace std;

class ClassDefinition; // forward declaration

class ProcedureNode; // forward declaration

class ProgramNode {
private:
   vector<ProcedureNode*> procedures;
   vector<ClassDefinition*> my_classes;
   vector<ProcedureNode*> methods;
   vector<Extern*> externs;
   vector<Statements*> statements_list;
   vector<string> global_variables;
   map<string, unsigned int> method_numbers;
   unsigned int next_methodnumber;

public:
   ProgramNode();
   vector<string> getGlobalVariables();
   ClassDefinition* getClass(string);
   unsigned int getMethodNumber(string);
   unsigned int assignMethodNumber(string);
   void addProcedure(ProcedureNode*);
   void addClass(ClassDefinition*);
   void addClassByName(string);
   void addMethodDefinition(ProcedureNode*);
   void addExtern(Extern*);
   void addStatements(Statements*);
   void clearStatements();
   void print(int);
   vector<ClassDefinition*> getClasses();
   vector<ProcedureNode*> getProcedures();
   void assignClassNumbers();
   vector<Extern*> getExterns();
   Extern* lookupExternal(string);
   void findMain();
};

#endif
