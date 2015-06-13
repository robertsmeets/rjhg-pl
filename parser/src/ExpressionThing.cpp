/*
 * ExpressionThing.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: robert
 */

#include "ExpressionThing.h"

#include <string>

using namespace std;

ExpressionThing::ExpressionThing(int a_type, string a_value) {

	mytype = a_type;
	value = a_value;

}

ExpressionThing::~ExpressionThing() {

}
//
// type:
// 1: operation
// 2: literal integer
// 3: variable name
// 4: procedure call
// 5: float
// 6: boolean
// 7: string
//

int ExpressionThing::getType() {
	return mytype;
}

string ExpressionThing::getValue() {
	return value;
}

void ExpressionThing::print(unsigned int level) {
	for (unsigned int i=0;i<level;i++)
	{
		cout << "+" ;
	}
	cout << "ExpressionThing ";
	cout << "[type=" << mytype << " value=" << value << "] ";
	cout << endl;
}
