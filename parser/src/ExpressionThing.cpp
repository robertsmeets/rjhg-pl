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

int ExpressionThing::getType() {
	return mytype;
}

string ExpressionThing::getValue() {
	return value;
}
