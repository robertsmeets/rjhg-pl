#include "ProcedureNode.h"

#include <iostream>

 /*
 * ProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */


using namespace std;

/**
 * constructor for call into bytecode
 */
ProcedureNode::ProcedureNode(string cname, string a_name, CommaSeparated* identifiers, Statements* some_statements)
{
   method_number =0;
   proc_address = 0;
   class_name = cname;
   name = a_name;
   parameters = identifiers;
   statements = some_statements;
   local_variables = new map<string, uint16_t>();
   built_in_method_nr = 0;
   cd = NULL;
}

/**
 * constructor for call to built in method
 */
ProcedureNode::ProcedureNode(string cname, string a_name, CommaSeparated* identifiers, uint16_t pbuilt_in_method_nr)
{
   method_number =0;
   proc_address = 0;
   class_name = cname;
   name = a_name;
   parameters = identifiers;
   local_variables = new map<string, uint16_t>();
   built_in_method_nr = pbuilt_in_method_nr;
   cd = NULL;
}

/**
 * destructor
 */
ProcedureNode::~ProcedureNode() {
   //
   // delete all the statements
   //
   /* parameters->erase(parameters->begin(), parameters->end());
   delete parameters;
   delete local_variables;
   for_each(statements.begin(), statements.end(),
         delete_pointed_to<Statement>);
   statements.clear(); */
}

/**
 * getter methods
 */

 string ProcedureNode::getName()
 {
    return name;
 }
 
 string ProcedureNode::getClassName()
 {
    return class_name;
 }

ClassDefinition* ProcedureNode::getClassDefinition()
{
   return cd;
}

/**
 * setter methods
 */
void ProcedureNode::setName(string a_name) {
   name = a_name;
}

void ProcedureNode::setProcAddress(uint16_t a) {
   proc_address = a;
}

void ProcedureNode::setClassDefinition(ClassDefinition* classDefinition)
{
   cd = classDefinition;
}

void ProcedureNode::addParameter(string a_parameter) {
}

void ProcedureNode::setStatements(vector<Statement*> some_statements) {
}

void ProcedureNode::setMethodNumber(int mn)
{
   method_number = mn;
}

uint16_t ProcedureNode::getProcAddress() {
   return proc_address;
}

vector<string>* ProcedureNode::getParameters() {
    return parameters->getIdentifiers();
}

map<string, uint16_t>* ProcedureNode::getLocalVariables() {
   return local_variables;
}


/**
 * find the index of an index variable by string
 *
 */
uint16_t ProcedureNode::assignLocalVariable(string s) {

   map<string, uint16_t>::iterator foundIter = local_variables->find(s);
   if (foundIter != local_variables->end()) {
      //
      // the variable already exists.
      //
      return local_variables->at(s);
   } else {
      //
      // the variable does not exist. add it.
      //
      uint16_t newval = local_variables->size();
      local_variables->insert(pair<string, uint16_t>(s, newval));
      return newval;
   }
}

/**
 * add a return statement if the procedure does not define one
 */
void ProcedureNode::fixReturn() {
   //
   // if return is missing, add it
   //
   vector<Expression*> sm = statements->getStatements();
   uint16_t sz = sm.size();
   bool addreturn = false;
   uint16_t linepos = 0;
   uint16_t charpos = 0;
   uint16_t abspos = 0;
    if ((sz == 0)) {
      addreturn = true;
   } else {
      Expression* last = sm.at(sz - 1);
   /* linepos = last->linepos;
      charpos = last->charpos;
      abspos = last->abspos; */
      addreturn = (last->stype() != "return");
   }
   if (addreturn) {
      ReturnNode* r = new ReturnNode(NULL);
      statements->addStatement(r);
   }
}

void ProcedureNode::print(int level) {
   for (int i=0;i<level;i++)
   {
      printf("+") ;
   } 
   printf("ProcedureNode \n" );
   if (built_in_method_nr != 0)
   {
      printf("built in method %d\n",built_in_method_nr);
   }
   else
   {
      statements->print(level+1);
   }
}

/**
 *
 * Look up the instance variable. 0 is a valid value
 * If the variable is not found, return -1
 *
 */
uint16_t ProcedureNode::getInstanceVarNum(string name) {
   if (cd == NULL) {
      return 0xffff;
   }
   uint16_t i = 0;
   for (auto it : cd->getInstanceVariables()) {
      if (it == name) {
         return i;
      }
      i++;
   }
   return 0xffff; 
}

Statements* ProcedureNode::getStatements()
{
   return statements;
}


/**
 * generate the code for a procedure
 */
void ProcedureNode::emit(CodeGenerator* cg, uint16_t here, ClassDefinition* a_cd) {
   cd = a_cd;
   if (built_in_method_nr == 0)
   {
      fixReturn(); 
      proc_address = here;
      //
      // emit all the statements for a procedure
      //
      vector<Expression*> expressions =statements->getStatements();
      for (auto const &it :expressions) {
           it->emit(cg,this);
      }
   }
}

uint16_t ProcedureNode::getMethodNumber() {
      return method_number;
   }

