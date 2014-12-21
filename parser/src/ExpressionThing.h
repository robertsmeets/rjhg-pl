/*
 * ExpressionThing.h
 *
 *  Created on: Dec 21, 2014
 *      Author: robert
 */

#ifndef EXPRESSIONTHING_H_
#define EXPRESSIONTHING_H_

#include <string>
using namespace std;
class ExpressionThing {
	int mytype;
	string value;
public:
	ExpressionThing(int,string);
	virtual ~ExpressionThing();
	int getType();
	string getValue();
};

#endif /* EXPRESSIONTHING_H_ */
