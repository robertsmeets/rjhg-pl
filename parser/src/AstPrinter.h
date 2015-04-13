/*
 * AstPrinter.h
 *
 *  Created on: Apr 13, 2015
 *      Author: robert
 */

#ifndef ASTPRINTER_H_
#define ASTPRINTER_H_

#include "ProgramNode.h"

class AstPrinter {
public:
	AstPrinter();
	virtual ~AstPrinter();
	void start(ProgramNode*);
};

#endif /* ASTPRINTER_H_ */
