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
   for (auto an_extern : externs) {
      delete an_extern;
   }
}

void pProgramNode::addClass(pClassDefinition* c) {
   my_classes.push_back(c);
}

void pProgramNode::addProcedure(pProcedureNode* c) {
   procedures.push_back(c);
}

void pProgramNode::addMethodDefinition(pProcedureNode* m) {
   pClassDefinition* c = getClass(m->getClassName());
   if (c==NULL)
   {
     puts("Class does not exist");
   }
   m->setMethodNumber(assignMethodNumber(m->getName()));
   c->add_method(m);
   methods.push_back(m);
}

void pProgramNode::addExtern(Extern* e)
{
   externs.push_back(e);
}

vector<Extern*> pProgramNode::getExterns()
{
   return externs;
}

void pProgramNode::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("pProgramNode\n" );
   for (Extern* an_extern:externs)
   {
      an_extern->print(level+1);
   }
   for (pClassDefinition* a_class : my_classes) {
      a_class->print(level + 1);
   }
   for (pProcedureNode* a_method : methods) {
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
   for (auto a_class:my_classes)
   {
      a_class->setClassNum(num);
      num++;
   } 
}

