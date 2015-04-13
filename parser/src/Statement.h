/*
 * Statement.h
 *
 *  Created on: Jan 9, 2015
 *      Author: robert
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <iostream>
#include <string>

using namespace std;
class CodeGenerator;
// forward declaration

class Statement {

public:
	unsigned int linepos;
	unsigned int charpos;
	unsigned int abspos;
	Statement();
	virtual ~Statement();
	virtual void emit(CodeGenerator*) = 0;
	virtual string stype() = 0;
	virtual void print()=0;
	unsigned int getLinepos();
	unsigned int getCharpos();
	unsigned int getAbspos();
};

#endif /* STATEMENT_H_ */
