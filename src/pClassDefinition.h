#ifndef PCLASSDEFINITION_H_
#define PCLASSDEFINITION_H_

#include <string>
#include <iostream>
#include <vector>

#include "pProcedureNode.h"

using namespace std;

class pClassDefinition {
private:
   string name;
   unsigned int classnum;
   vector<string> instance_variables;
   vector <pProcedureNode*> methods;
   
public:
   pClassDefinition(string);
   virtual ~pClassDefinition();
   void print(int);
   string getName();
   vector <pProcedureNode*> getMethods();
   void add_instance_variable(string);
   void add_method(pProcedureNode*);
   vector<string> getInstanceVariables();
   unsigned int getClassNum();
   pProcedureNode* findMethod(string);
   void setClassNum(int);
};

#endif
