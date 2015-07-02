#include "pProgramNode.h"

pProgramNode::pProgramNode() {
	cout << "pProgramNode initialized: " << this << endl;
}

pProgramNode::~pProgramNode() {
}

void pProgramNode::addClass(pClassDefinition* c) {
	my_classes.push_back(c);
}

void pProgramNode::addProcedure(pProcedureNode* c) {
	cout << "adding a procedure" << endl;
	procedures.push_back(c);
}

void pProgramNode::addMethodDefinition(pMethodDefinition* c) {
	cout << "in addMethodDefinition()" << endl;
	methods.push_back(c);
	cout << "now there are " << methods.size() << " methods" << endl;
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

