/*
 * Statement.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: robert
 */

#include "Statement.h"

using namespace std;

Statement::Statement() {
	linepos = 0;
	charpos = 0;
	abspos = 0;
}

Statement::~Statement() {
}

unsigned int Statement::getLinepos() {
	return linepos;
}

unsigned int Statement::getCharpos() {
	return charpos;
}

unsigned int Statement::getAbspos() {
	return abspos;
}
