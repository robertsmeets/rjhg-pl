#ifndef CLASSDEFINITION_H_
#define CLASSDEFINITION_H_

#include <string>
#include "stdio.h"
#include <vector>

#include "ProcedureNode.h"

using namespace std;

class ProcedureNode; // forward declaration

class ClassDefinition {
private:
   string name;
   unsigned int classnum;
   vector<string> instance_variables;
   vector <ProcedureNode*> methods;
   
public:
   ClassDefinition(string);
   virtual ~ClassDefinition();
   void print(int);
   string getName();
   vector <ProcedureNode*> getMethods();
   void add_instance_variable(string);
   void add_method(ProcedureNode*);
   vector<string> getInstanceVariables();
   unsigned int getClassNum();
   ProcedureNode* findMethod(string);
   void setClassNum(int);
};

#endif
