/*
 * ClassDefinition.cpp
 *
 *  Created on: Mar 23, 2015
 *      Author: robert
 */

#include "ClassDefinition.h"

ClassDefinition::ClassDefinition(string a_name, unsigned int num) {
	name = a_name;
	classnum = num;
}

ClassDefinition::~ClassDefinition() {

}
string ClassDefinition::getName() {
	return name;
}

vector<string> ClassDefinition::getInstanceVariables() {
	return instance_variables;
}

void ClassDefinition::add_instance_variable(string i) {
	instance_variables.push_back(i);
}

void ClassDefinition::add_method(ProcedureNode* m) {
	cout << "adding a method " << endl;
	methods.push_back(m);
}

unsigned int ClassDefinition::getClassNum() {
	return classnum;
}

vector<ProcedureNode*> ClassDefinition::getMethods() {
	return methods;
}
