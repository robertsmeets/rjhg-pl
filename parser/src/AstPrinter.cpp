/*
 * AstPrinter.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: robert
 */

#include "AstPrinter.h"

AstPrinter::AstPrinter() {


}

AstPrinter::~AstPrinter() {

}

void AstPrinter::start(ProgramNode* pn)
{
	pn->print();
}
