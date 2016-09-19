#include "ClassDefinition.h"


using namespace std;

ClassDefinition::ClassDefinition(string class_name,CommaSeparated* instance_vars) {
   classnum = 0;
   name = class_name;
   vector<string>* v =  instance_vars->getIdentifiers();
   for (auto i: *v)
{
      instance_variables.push_back(i);
} 
}

ClassDefinition::~ClassDefinition() {
}

string ClassDefinition::getName() {
   return name;
}

void ClassDefinition::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   } 
   printf("ClassDefinition ");
   printf("There are %d instance variables",instance_variables.size());
   printf("\n" );
}

vector<ProcedureNode*> ClassDefinition::getMethods() {
   return methods;
}

vector<string> ClassDefinition::getInstanceVariables() {
   return instance_variables;
}

void ClassDefinition::add_instance_variable(string i) {
   instance_variables.push_back(i);
}

void ClassDefinition::add_method(ProcedureNode* m) {
   methods.push_back(m);
}

unsigned int ClassDefinition::getClassNum() {
   return classnum;
}

void ClassDefinition::setClassNum(int n) {
   classnum = n;
}

ProcedureNode* ClassDefinition::findMethod(string method_name) {
   for (auto a_method : methods) {
      if (a_method->getName() == method_name) {
         return a_method;
      }
   }
   return NULL;
}

