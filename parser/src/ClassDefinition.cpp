/*
 * ClassDefinition.cpp
 *
 *  Created on: Mar 23, 2015
 *      Author: robert
 */

#include "ClassDefinition.h"

ClassDefinition::ClassDefinition(string a_name) {
	name = a_name;
}

ClassDefinition::~ClassDefinition() {

}
string ClassDefinition::getName()
{
	return name;
}

void ClassDefinition::add_instance_variable(string i) {
	instance_variables.push_back(i);
}

void ClassDefinition::add_method(ProcedureNode* m) {
	methods.push_back(m);
}
