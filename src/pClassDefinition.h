#ifndef PCLASSDEFINITION_H_
#define PCLASSDEFINITION_H_

#include <string>
#include <iostream>
#include <vector>

#include "pMethodDefinition.h"

using namespace std;

class pMethodDefinition; // forward declaration

class pClassDefinition {
private:
   string name;
   unsigned int classnum;
   vector<string> instance_variables;
   vector <pMethodDefinition*> methods;
   
public:
   pClassDefinition(string);
   virtual ~pClassDefinition();
   void print(int);
   string getName();
   vector <pMethodDefinition*> getMethods();
   void add_instance_variable(string);
   void add_method(pMethodDefinition*);
   vector<string> getInstanceVariables();
   unsigned int getClassNum();
   pMethodDefinition* findMethod(string);
   void setClassNum(int);
};

#endif
