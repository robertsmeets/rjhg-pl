/*
 * ProgramNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProgramNode.h"

ProgramNode::ProgramNode() {
   next_methodnumber = 1;
   //
   // add the vector class
   // 
   addClass(new ClassDefinition("vector",new CommaSeparated()));
   addMethodDefinition(new ProcedureNode("vector","add",new CommaSeparated(),1));
}

ProgramNode::~ProgramNode() {
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

void ProgramNode::addClass(ClassDefinition* c) {
   my_classes.push_back(c);
}

void ProgramNode::addProcedure(ProcedureNode* c) {
   procedures.push_back(c);
}

void ProgramNode::addMethodDefinition(ProcedureNode* m) {
   ClassDefinition* c = getClass(m->getClassName());
   if (c==NULL)
   {
     puts("Class does not exist");
   }
   m->setMethodNumber(assignMethodNumber(m->getName()));
   c->add_method(m);
   methods.push_back(m);
}

void ProgramNode::addExtern(Extern* e)
{
   externs.push_back(e);
}

vector<Extern*> ProgramNode::getExterns()
{
   return externs;
}

void ProgramNode::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("ProgramNode\n" );
   for (Extern* an_extern:externs)
   {
      an_extern->print(level+1);
   }
   for (ClassDefinition* a_class : my_classes) {
      a_class->print(level + 1);
   }
   for (ProcedureNode* a_method : methods) {
      a_method->print(level + 1);
   }
   for (ProcedureNode* a_proc : procedures) {
      a_proc->print(level + 1);
   }
}

vector <ClassDefinition*> ProgramNode::getClasses()
{
   return my_classes;
}

vector <ProcedureNode*> ProgramNode::getProcedures()
{
   return procedures;
}


ClassDefinition* ProgramNode::getClass(string class_name) {
   for (auto a_class : my_classes) {
      if (a_class->getName() == class_name) {
         return a_class;
      }
   }
   return NULL;
}

unsigned int ProgramNode::assignMethodNumber(string method_name) {
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

unsigned int ProgramNode::getMethodNumber(string method_name) {
   return method_numbers[method_name];
}

/**
 *
 * assign class numbers
 *
 **/
void ProgramNode::assignClassNumbers()
{
   int num = 1;
   for (auto a_class:my_classes)
   {
      a_class->setClassNum(num);
      num++;
   } 
}

Extern* ProgramNode::lookupExternal(string name)
{
   for (auto an_extern:externs)
   {
        if (an_extern->getName() == name)
        {
            return an_extern;
        }
   }
}

