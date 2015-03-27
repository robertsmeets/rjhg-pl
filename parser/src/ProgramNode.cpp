/*
 * ProgramNode.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ProgramNode.h"
#include "ClassDefinition.h"

ProgramNode::ProgramNode() {

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
		if (a_class->getName() == class_name) {
			return a_class;
		}
	}
	return NULL;
}
