/*
 * VariableValue.cpp
 *
 *  Created on: Jul 1, 2015
 *   Author: Robert
 */

#include "VariableValue.h"

VariableValue::VariableValue(string c) {
   value = c;
}

VariableValue::~VariableValue() {

}

void VariableValue::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("VariableValue <%s>\n",value.c_str());
}

void VariableValue::emit(CodeGenerator* cg, ProcedureNode* pn) {
   map<string, uint16_t>* local_variables;
   map<string, uint16_t>::iterator foundIter;
   vector<string>* parameters;
   vector<string>::iterator it2;
   //
   // now we have to look up the variable name.
   // Can be either a local variable, a parameter name or an instance variable
   //
   local_variables = pn->getLocalVariables();
   foundIter = local_variables->find(value);
   if (foundIter == local_variables->end()) {
      parameters = pn->getParameters();
      for (it2 = parameters->begin(); it2 != parameters->end(); ++it2) {
         if ((*it2) == value) {
            uint16_t number = it2 - parameters->begin();
            //
            // it is a parameter
            //
            cg->emit(3, 0, number, NULL); // LOD
            break;
         }
      }
      if (it2 == parameters->end()) {
         //
         // look for instance variable
         //
         uint16_t j = pn->getInstanceVarNum(value);
         if (j != 0xffff) {
            cg->emit(13, j, pn->getParameters()->size(), NULL); // LDI
            return;
         }
      }
   } else {
      //
      // it is a local variable
      //
      int sz1 = pn->getParameters()->size();
      int av = local_variables->at(value);
      // add the parameters plus one for self
      int offset = sz1 + av;
      if (pn->getClassDefinition() != NULL)
      {
          offset++;
      }
      cg->emit(3, 0, offset, NULL); // LOD
   }

}

string VariableValue::stype() {
   return "VariableValue";
}

string VariableValue::getName() {
   return value;
}
void VariableValue::setTopLevel() {}
