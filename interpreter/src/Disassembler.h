/*
 * Disassembler.h
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include <vector>

using namespace std;

class Disassembler {
public:
	Disassembler();
	virtual ~Disassembler();
	void start(vector<char>);
	string decode(char,unsigned short, unsigned short);
};

#endif /* DISASSEMBLER_H_ */
