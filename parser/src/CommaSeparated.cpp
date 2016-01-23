#include "CommaSeparated.h"

#include <iostream>

using namespace std;

CommaSeparated::CommaSeparated()
{
   identifiers = new vector<string>();
}

void CommaSeparated::addIdentifier(char* s)
{
   string ss(s);
   identifiers->push_back(ss);
}

void CommaSeparated::print(int level)
{
   for (int i = 0; i < level; i++) {
	cout << "+";
   }
   cout << "CommaSeparated" << endl;
}

vector<string>* CommaSeparated::getIdentifiers()
{
   return identifiers;
}
