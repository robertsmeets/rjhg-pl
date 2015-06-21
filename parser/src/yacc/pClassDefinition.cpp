#include "pClassDefinition.h"

#include <iostream>

using namespace std;

pClassDefinition::pClassDefinition(string class_name) {
	cout << "class definition " << class_name << endl;
}

pClassDefinition::~pClassDefinition() {
}

void pClassDefinition::print(int level) {
	{for (unsigned int i = 0; i < level; i++) {
					cout << "+";
				}
	cout << "pClassDefinition" << endl;			}

}
