/*
 * VariableValue.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: Robert
 */

#include "VariableValue.h"

VariableValue::VariableValue(string c) {
	value = c;
}

VariableValue::~VariableValue() {

}

void VariableValue::print(int level) {
	for (int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "VariableValue " + value << endl;
}


void VariableValue::emit(CodeGenerator* cg, pProcedureNode* pn)
{
   cg->emit(3,0,0,NULL);
}

string VariableValue::stype() { return "VariableValue" ; }


string VariableValue::getName()
{
   return value;
}
