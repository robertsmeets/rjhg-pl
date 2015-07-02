#include "pProgramNode.h"



pProgramNode::pProgramNode() {
cout << "pProgramNode initialized: " << this << endl;

}

pProgramNode::~pProgramNode() {
}

void pProgramNode::addClass(pClassDefinition* c) {
	cout << "hello I am pProgramNode " << this << endl;
	cout << "adding class" << endl;
	cout << "received a class with name " << c->getName() << endl;
	cout << "classes size = " << my_classes.size() << endl;

	my_classes.push_back(c);
	cout << "after adding class" << endl;

}

void pProgramNode::addProcedure(pProcedureNode* c) {
	cout << "adding procedure" << endl;
	procedures.push_back(c);
	cout << "after adding procedure" << endl;

}

void pProgramNode::addMethodDefinition(pMethodDefinition* c) {
	cout << "adding method" << endl;
	methods.push_back(c);
	cout << "after adding method" << endl;

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

