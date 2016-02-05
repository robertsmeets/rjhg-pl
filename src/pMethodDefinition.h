#ifndef PMETHODDEFINITION_H_
#define PMETHODDEFINITION_H_

#include <string>
#include <map>

#include "Statement.h"
#include "Statements.h"
#include "pClassDefinition.h"
#include "pProgramNode.h"

using namespace std;

class pClassDefinition; // forward declaration
class pProgramNode; // forward declaration

class pMethodDefinition {
   string class_name;
   string method_name;
   Statements* statements;
   pClassDefinition* cd;
   vector<string>* parameters;
   vector<string> instance_variables;
   map<string, uint16_t>* local_variables;
   uint16_t proc_address;
   uint16_t method_number;
   pProgramNode* pn;
public:
   pMethodDefinition(string,string,Statements*);
   virtual ~pMethodDefinition();
   void print(int);
   void setProcAddress(uint16_t);
   void setName(string);
   string getClassName();
   string getName();
   map<string, uint16_t>* getLocalVariables();
   uint16_t getProcAddress();
   vector<string>* getParameters();
   string getFullMethodName();
   void addParameter(string);
   uint16_t assignLocalVariable(string);
   uint16_t get_proc_address();
   vector<Statement*> getStatements();
   void setStatements(vector<Statement*>);
   void fixReturn();
   void setMethodNumber(uint16_t);
   uint16_t getMethodNumber();
   int getInstanceVarNum(string);

};

#endif

