#include "pMethodDefinition.h"

#include <iostream>

using namespace std;

pMethodDefinition::pMethodDefinition(string a_class_name, string a_method_name, Statements* a_statements) {
   class_name = a_class_name;
   method_name = a_method_name;
   statements = a_statements;
   parameters = new vector<string>();
   local_variables = new map<string, uint16_t>();
}

pMethodDefinition::~pMethodDefinition() {
}

void pMethodDefinition::print(int level) {
   for (unsigned int i = 0; i < level; i++) {
      cout << "+";
   }
   cout << "pMethodDefinition" << endl;
   statements->print(level + 1);
}


/**
 * setter methods
 */
void pMethodDefinition::setName(string a_name) {
   method_name = a_name;
}

void pMethodDefinition::setProcAddress(uint16_t a) {
   proc_address = a;
}

void pMethodDefinition::addParameter(string a_parameter) {
   parameters->push_back(a_parameter);
}

void pMethodDefinition::setStatements(vector<Statement*> some_statements) {
//   statements = some_statements;
}

/**
 * getter methods
 */
string pMethodDefinition::getClassName()
{
   return class_name;
}

string pMethodDefinition::getName() {
   return method_name;
}

uint16_t pMethodDefinition::getProcAddress() {
   return proc_address;
}

vector<string>* pMethodDefinition::getParameters() {
   return parameters;
}

map<string, uint16_t>* pMethodDefinition::getLocalVariables() {
   return local_variables;
}

vector<Statement*> pMethodDefinition::getStatements() {
//   return statements;
}

void pMethodDefinition::setMethodNumber(uint16_t num)
{
   method_number = num;
}

uint16_t pMethodDefinition::getMethodNumber() {
   return method_number;
}

string pMethodDefinition::getFullMethodName() {
   return class_name + "." + method_name;
}

/**
 * find the index of an index variable by string
 *
 */
uint16_t pMethodDefinition::assignLocalVariable(string s) {

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
void pMethodDefinition::fixReturn() {
   //
   // if return is missing, add it
   //
/*   uint16_t sz = statements.size();
   bool addreturn = false;
   uint16_t linepos = 0;
   uint16_t charpos = 0;
   uint16_t abspos = 0;
   if ((sz == 0)) {
      addreturn = true;
   } else {
      Statement* last = statements.at(sz - 1);
      linepos = last->linepos;
      charpos = last->charpos;
      abspos = last->abspos;
      addreturn = (last->stype() != "return");
   }
   if (addreturn) {
      ExpressionNode en;
      statements.push_back(
            new ReturnNode(this, en, linepos, charpos, abspos));
   } */
}


int pMethodDefinition::getInstanceVarNum(string name) {
   if (cd == NULL) {
      return -1;
   }
   int i = 0;
   for (auto it : cd->getInstanceVariables()) {
      if (it == name) {
         return i;
      }
      i++;
   }
   return -1;
}




















