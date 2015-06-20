#include "pClassDefinition.h"

#include <iostream>

using namespace std;

pClassDefinition::pClassDefinition(string class_name) {
	cout << "class definition " << class_name << endl;
}

pClassDefinition::~pClassDefinition() {
}

void pClassDefinition::print(int i) {
	cout << "pClassDefinition" << endl;
}
