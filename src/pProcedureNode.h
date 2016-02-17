#ifndef PPROCEDURENODE_H_
#define PPROCEDURENODE_H_

 /*
 * pProcedureNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "stdint.h"

#include "Statement.h"
#include "PException.h"
#include "pReturn.h"
#include "CFunctions.h"
#include "pClassDefinition.h"
#include "pProgramNode.h"
#include "Statements.h"
#include "CommaSeparated.h"

using namespace std;

class Statements; // forward declaration

class pProgramNode;
// forward declaration

class Statement;
// forward declaration

class pProcedureNode {
   string class_name;
   string name;
   pClassDefinition* cd;
   CommaSeparated* parameters;
   vector<string> instance_variables;
   Statements* statements;
   map<string, uint16_t>* local_variables;
   uint16_t proc_address;
   uint16_t method_number;

public:
   pProcedureNode(string, string, CommaSeparated*, Statements*);
   virtual ~pProcedureNode();
   void setName(string);
   string getName();
   map<string, uint16_t>* getLocalVariables();
   string getClassName();
   void setProcAddress(uint16_t);
   void setMethodNumber(int);
   uint16_t getProcAddress();
   vector<string>* getParameters();
   string getFullMethodName();
   void addParameter(string);
   uint16_t assignLocalVariable(string);
   uint16_t get_proc_address();
   Statements* getStatements();
   void setStatements(vector<Statement*>);
   void fixReturn();
   uint16_t getMethodNumber();
   void print(int);
   int getInstanceVarNum(string);
   void emit(CodeGenerator*);
   
};

#endif /* PPROCEDURENODE_H_ */


