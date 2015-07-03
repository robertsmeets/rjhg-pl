#include "pMethodDefinition.h"

#include <iostream>

using namespace std;

pMethodDefinition::pMethodDefinition(string a_class_name, string a_method_name,
		Statements* a_statements) {
	class_name = a_class_name;
	method_name = a_method_name;
	statements = a_statements;
}

pMethodDefinition::~pMethodDefinition() {
}

void pMethodDefinition::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "pMethodDefinition" << endl;
	statements->print(level + 1);
}
