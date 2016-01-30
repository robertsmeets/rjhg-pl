/*
 * ProgramNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "pProgramNode.h"

pProgramNode::pProgramNode() {
   next_methodnumber = 1;
}

pProgramNode::~pProgramNode() {
   cout << "--- pProgramNode DESTRUCTOR" << endl;
   for (auto a_procedure : procedures) {
      delete a_procedure;
   }
   for (auto a_class : my_classes) {
      delete a_class;
   }
}

void pProgramNode::addClass(pClassDefinition* c) {
   my_classes.push_back(c);
}

void pProgramNode::addProcedure(pProcedureNode* c) {
   procedures.push_back(c);
}

void pProgramNode::addMethodDefinition(pMethodDefinition* c) {
   methods.push_back(c);
}

void pProgramNode::print(int level) {
   for (int i = 0; i < level; i++) {
      cout << "+";
   }
   cout << "pProgramNode" << endl;
   for (pClassDefinition* a_class : my_classes) {
      a_class->print(level + 1);
   }
   for (pMethodDefinition* a_method : methods) {
      a_method->print(level + 1);
   }
   for (pProcedureNode* a_proc : procedures) {
      a_proc->print(level + 1);
   }
}

vector <pClassDefinition*> pProgramNode::getClasses()
{
   return my_classes;
}

vector <pProcedureNode*> pProgramNode::getProcedures()
{
   return procedures;
}


pClassDefinition* pProgramNode::getClass(string class_name) {
   for (auto a_class : my_classes) {
      if (a_class->getName() == class_name) {
         return a_class;
      }
   }
   return NULL;
}

unsigned int pProgramNode::assignMethodNumber(string method_name) {
   auto it = method_numbers.find(method_name);
   unsigned int i;
   if (it == method_numbers.end()) {
      method_numbers[method_name] = next_methodnumber;
      i = next_methodnumber;
      next_methodnumber++;
   } else {
      i = it->second;
   }
   return i;
}

unsigned int pProgramNode::getMethodNumber(string method_name) {
   return method_numbers[method_name];
}

/**
 *
 * assign class numbers
 *
 **/
void pProgramNode::assignClassNumbers()
{
   int num = 1;
   cout << "interesting" << endl;
   for (auto a_class:my_classes)
   {
      cout << "setting class " << a_class->getName() << " to " << num << endl;
      a_class->setClassNum(num);
      num++;
   } 
}

