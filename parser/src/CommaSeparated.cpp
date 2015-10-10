#include "CommaSeparated.h"

#include <iostream>

using namespace std;

void CommaSeparated::addIdentifier(string){};
void CommaSeparated::print(int level)
{for (unsigned int i = 0; i < level; i++) {
	cout << "+";
}
cout << "CommaSeparated" << endl;
}
