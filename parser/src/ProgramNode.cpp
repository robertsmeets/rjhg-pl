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
		cout << "a_class " << a_class->getName() << endl;
		if (a_class->getName() == class_name) {
			cout << " match" << endl;
			return a_class;
		}
		else
		{
			cout << "no match" << endl;
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
