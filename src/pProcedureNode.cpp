#include "pProcedureNode.h"

#include <iostream>

 /*
 * pProcedureNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */


using namespace std;


/**
 * constructor
 */
pProcedureNode::pProcedureNode(string cname, string a_name, CommaSeparated* identifiers, Statements* some_statements)
{
   method_number =0;
   proc_address = 0;
   class_name = cname;
   name = a_name;
   parameters = identifiers;
   statements = some_statements;
   local_variables = new map<string, uint16_t>();
   cd = NULL;
}


/**
 * destructor
 */
pProcedureNode::~pProcedureNode() {
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

 string pProcedureNode::getName()
 {
    return name;
 }
 
 string pProcedureNode::getClassName()
 {
    return class_name;
 }

/**
 * setter methods
 */
void pProcedureNode::setName(string a_name) {
   name = a_name;
}

void pProcedureNode::setProcAddress(uint16_t a) {
   proc_address = a;
}

void pProcedureNode::addParameter(string a_parameter) {
   // parameters->push_back(a_parameter);
}

void pProcedureNode::setStatements(vector<Statement*> some_statements) {
   // statements = some_statements;
}

void pProcedureNode::setMethodNumber(int mn)
{
   method_number = mn;
}

uint16_t pProcedureNode::getProcAddress() {
   return proc_address;
}

vector<string>* pProcedureNode::getParameters() {
    return parameters->getIdentifiers();
}

map<string, uint16_t>* pProcedureNode::getLocalVariables() {
   return local_variables;
}


/**
 * find the index of an index variable by string
 *
 */
uint16_t pProcedureNode::assignLocalVariable(string s) {

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
void pProcedureNode::fixReturn() {
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
      pReturn* r = new pReturn(NULL);
      statements->addStatement(r);
   }
}

void pProcedureNode::print(int level) {
   for (int i=0;i<level;i++)
   {
      printf("+") ;
   } 
   printf("pProcedureNode \n" );
   statements->print(level+1);
}

int pProcedureNode::getInstanceVarNum(string name) {
   if (cd == NULL) {
      return 0;
   }
   int i = 0;
   for (auto it : cd->getInstanceVariables()) {
      if (it == name) {
         return i;
      }
      i++;
   }
   return 0; 
}

Statements* pProcedureNode::getStatements()
{
   return statements;
}


/**
 * generate the code for a procedure
 */
void pProcedureNode::emit(CodeGenerator* cg) {
   //
   // emit all the statements for a procedure
   //
   vector<Expression*> expressions =statements->getStatements();
   for (auto const &it :expressions) {
           it->emit(cg,this);
   }
}

uint16_t pProcedureNode::getMethodNumber() {
      return method_number;
   }


