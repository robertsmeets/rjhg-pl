/*
 * pProgramNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */
#ifndef PPROGRAMNODE_H_
#define PPROGRAMNODE_H_

#include "pProcedureNode.h"
#include "pClassDefinition.h"
#include "Extern.h"

#include <string>
#include <vector>

using namespace std;

class pClassDefinition; // forward declaration

class pProcedureNode; // forward declaration

class pProgramNode {
private:
   vector<pProcedureNode*> procedures;
   vector<pClassDefinition*> my_classes;
   vector<pProcedureNode*> methods;
   vector<Extern*> externs;
   map<string, unsigned int> method_numbers;
   unsigned int next_methodnumber;

public:
   pProgramNode();
   virtual ~pProgramNode();
   pClassDefinition* getClass(string);
   unsigned int getMethodNumber(string);
   unsigned int assignMethodNumber(string);
   void addProcedure(pProcedureNode*);
   void addClass(pClassDefinition*);
   void addClassByName(string);
   void addMethodDefinition(pProcedureNode*);
   void addExtern(Extern*);
   void print(int);
   vector<pClassDefinition*> getClasses();
   vector<pProcedureNode*> getProcedures();
   void assignClassNumbers();
   vector<Extern*> getExterns();
   Extern* lookupExternal(string);
};

#endif
