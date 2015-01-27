/*
 * Disassembler.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#include <iostream>
#include <sstream>

#include "Disassembler.h"

using namespace std;

Disassembler::Disassembler() {

}

Disassembler::~Disassembler() {

}

void Disassembler::start(vector<char> buffer) {
	for (unsigned int i = 0; i < buffer.size(); i += 5) {
		char f = buffer[i];
		unsigned short l = (buffer[i + 1] & 255) + (buffer[i + 2] << 8);
		unsigned short a = (buffer[i + 3] & 255) + (buffer[i + 4] << 8);
		string s = decode(f, l, a);
		cout << "i=" << i << " "<< s << endl;
	}
}

string Disassembler::decode(char f, unsigned short l, unsigned short a) {
	//
	// opcode definitions
	//
	string sf, sl, sa;
	stringstream out1;
	out1 << l;
	sl = out1.str();
	stringstream out2;
	out2 << a;
	sa = out2.str();
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		sf = "LIT";
		break;
	case 2: // opr
		sf = "OPR";
		switch (a) {
		case 0:
			sa = "RET";
			break;
		case 1:
			sa = "UNARY MINUS";
			break;
		case 2:
			sa = "PLUS";
			break;
		case 3:
			sa = "MINUS";
			break;
		case 4:
			sa = "MUL";
			break;
		case 5:
			sa = "DIV";
			break;
		case 6:
			sa = "MOD";
			break;
		case 8:
			sa = "EQ";
			break;
		case 9:
			sa = "NE";
			break;
		case 10:
			sa = "LT";
			break;
		case 11:
			sa = "GE";
			break;
		case 12:
			sa = "GT";
			break;
		case 13:
			sa = "LE";
			break;
		default:
			sa = "unexpected A value: " + a;
			break;
		}
		break;
	case 3:
		sf = "LOD";
		//lod: copy a local variable on top of the stack
		break;
	case 4:
		// sto: pop a value from the stack and put it in a local variable
		sf = "STO";
		break;
	case 5:
		//cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		sf = "CAL";
		break;
	case 6:
		// int:
		sf = "INT";
		break;
	case 7:
		// jmp
		sf = "JMP";
		break;
	case 8:
		// jpc
		sf = "JPC";
		break;
	case 9:
			sf = "PRINT";
			break;
	case 10:
			sf = "MININT";
			break;
	default:
		sf = "------------------------------------> unexpected F value: " + int(f);
		break;
	}
	string total = sf + " " + sl + "," + sa;
	return total;
}
