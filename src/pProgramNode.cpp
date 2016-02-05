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

void pProgramNode::addMethodDefinition(pMethodDefinition* m) {
   pClassDefinition* c = getClass(m->getClassName());
   if (c==NULL)
   {
      throw new PException("Class " + m->getClassName() + " does not exist");
   }
   m->setMethodNumber(assignMethodNumber(m->getName()));
   c->add_method(m);
   methods.push_back(m);
   for (auto it = method_numbers.begin(); it != method_numbers.end(); it++)
   {
      cout << it->first <<  it->second << endl;
   }
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
   cout << "assignMethodNumber("<<method_name<<") " << endl;
   auto it = method_numbers.find(method_name);
   unsigned int i;
   if (it == method_numbers.end()) {
      method_numbers[method_name] = next_methodnumber;
      i = next_methodnumber;
      cout << "new number = " << i << endl;
      next_methodnumber++;
   } else {
      i = it->second;
      cout << "already existing " << i << endl;
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
   for (auto a_class:my_classes)
   {
      a_class->setClassNum(num);
      num++;
   } 
}

