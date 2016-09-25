#ifndef PROCEDURENODE_H_
#define PROCEDURENODE_H_

 /*
 * ProcedureNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include <string>
#include <vector>
#include "stdio.h"
#include <map>
#include <algorithm>
#include "stdint.h"

#include "Statement.h"

#include "CFunctions.h"
#include "ClassDefinition.h"
#include "Statements.h"
#include "CommaSeparated.h"
#include "ProgramNode.h"
#include "ReturnNode.h"

using namespace std;

class Statements; // forward declaration

class ProgramNode;
// forward declaration

class Statement;
// forward declaration

class ProcedureNode {
   string class_name;
   string name;
   ClassDefinition* cd;
   CommaSeparated* parameters;
   vector<string> instance_variables;
   Statements* statements;
   map<string, uint16_t>* local_variables;
   uint16_t proc_address;
   uint16_t method_number;

public:
   ProcedureNode(string, string, CommaSeparated*, Statements*);
   virtual ~ProcedureNode();
   ClassDefinition* getClassDefinition();
   vector<string>* getParameters();
   map<string, uint16_t>* getLocalVariables();
   string getClassName();
   string getName();
   uint16_t getProcAddress();
   string getFullMethodName();
   int getInstanceVarNum(string);
   uint16_t get_proc_address();
   uint16_t getMethodNumber();
   Statements* getStatements();
   void setName(string);
   void setProcAddress(uint16_t);
   void setMethodNumber(int);
   void addParameter(string);
   uint16_t assignLocalVariable(string);
   void setStatements(vector<Statement*>);
   void fixReturn();
   void print(int);
   void emit(CodeGenerator*);
   void setClassDefinition(ClassDefinition*);
};

#endif /* PROCEDURENODE_H_ */


