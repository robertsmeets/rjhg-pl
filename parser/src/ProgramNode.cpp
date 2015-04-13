/*
 * ProgramNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProgramNode.h"
#include "ClassDefinition.h"

ProgramNode::ProgramNode() {
	next_methodnumber = 1;
}

ProgramNode::~ProgramNode() {
	for (auto a_procedure : procedures) {
		delete a_procedure;
	}
	for (auto a_class : classes) {
		delete a_class;
	}
}

void ProgramNode::addProcedure(ProcedureNode* pn) {
	cout << "--- ADDING PROCEDURE " << pn->getName() << endl;
	procedures.push_back(pn);
}

vector<ProcedureNode*> ProgramNode::getProcedures() {
	return procedures;
}

void ProgramNode::addClass(ClassDefinition* cd) {
	classes.push_back(cd);
}

vector<ClassDefinition*> ProgramNode::getClasses() {
	return classes;
}

ClassDefinition* ProgramNode::getClass(string class_name) {
	for (auto a_class : classes) {
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
	cout << " getMethodNumber for <" << method_name << "> is "
			<< method_numbers[method_name] << endl;
	return method_numbers[method_name];
}


void ProgramNode::print()
{
	cout << "ProgramNode" << endl;
	for (auto cd:classes)
	{
		cd->print();
	}
	for (auto p:procedures)
	{
		p->print();
	}

}
