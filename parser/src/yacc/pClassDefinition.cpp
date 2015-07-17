#include "pClassDefinition.h"

#include <iostream>

using namespace std;

pClassDefinition::pClassDefinition(string class_name) {
	name = class_name;
}

pClassDefinition::~pClassDefinition() {
}

string pClassDefinition::getName() {
	return name;
}

void pClassDefinition::print(int level) {
	{
		for (unsigned int i = 0; i < level; i++) {
			cout << "+";
		}
		cout << "pClassDefinition" << endl;
	}

}
