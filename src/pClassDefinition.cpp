#include "pClassDefinition.h"


using namespace std;

pClassDefinition::pClassDefinition(string class_name) {
   classnum = 0;
   name = class_name;
}

pClassDefinition::~pClassDefinition() {
}

string pClassDefinition::getName() {
   return name;
}

void pClassDefinition::print(int level) {
   for (int i = 0; i < level; i++) {
      cout << "+";
   } 
   cout << "pClassDefinition " << name << endl;
}

vector<pProcedureNode*> pClassDefinition::getMethods() {
   return methods;
}

vector<string> pClassDefinition::getInstanceVariables() {
   return instance_variables;
}

void pClassDefinition::add_instance_variable(string i) {
   instance_variables.push_back(i);
}

void pClassDefinition::add_method(pProcedureNode* m) {
   methods.push_back(m);
}

unsigned int pClassDefinition::getClassNum() {
   return classnum;
}

void pClassDefinition::setClassNum(int n) {
   classnum = n;
}

pProcedureNode* pClassDefinition::findMethod(string method_name) {
   for (auto a_method : methods) {
      if (a_method->getName() == method_name) {
         return a_method;
      }
   }
   return NULL;
}

