/*
 * Disassembler.h
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include <vector>

#include "DebugInfo.h"


#include <ctype.h>
#include <stdio.h>


using namespace std;

class Disassembler {
	DebugInfo* di;
	unsigned int i;
public:
	Disassembler();
	virtual ~Disassembler();
	void start(char*, unsigned int, DebugInfo*);
	string decode(char, unsigned short, unsigned short);
	void hexdump(char*, unsigned int);
};

#endif /* DISASSEMBLER_H_ */
