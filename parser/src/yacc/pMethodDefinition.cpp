#include "pMethodDefinition.h"

#include <iostream>

using namespace std;

pMethodDefinition::pMethodDefinition(string class_nema,string method_name)
{
}


pMethodDefinition::~pMethodDefinition()
{
}

void pMethodDefinition::print(int level) {
	{for (unsigned int i = 0; i < level; i++) {
						cout << "+";
					}
	cout << "pMethodDefinition" << endl;
			}
}
