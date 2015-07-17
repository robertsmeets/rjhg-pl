#include "pProgramNode.h"

pProgramNode::pProgramNode() {
}

pProgramNode::~pProgramNode() {
}

void pProgramNode::addClass(pClassDefinition* c) {
	my_classes.push_back(c);
}

void pProgramNode::addProcedure(pProcedureNode* c) {
	procedures.push_back(c);
}

void pProgramNode::addMethodDefinition(pMethodDefinition* c) {
	methods.push_back(c);
}

void pProgramNode::print(int level) {
	for (int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "pProgramNode" << endl;
	for (pClassDefinition* a_class : my_classes) {
		a_class->print(level + 1);
	}
	for (pMethodDefinition* a_method : methods) {
		a_method->print(level + 1);
	}
	for (pProcedureNode* a_proc : procedures) {
		a_proc->print(level + 1);
	}
}

