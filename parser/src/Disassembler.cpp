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
	di = NULL;
	i = 0;
}

Disassembler::~Disassembler() {
}

void Disassembler::start(char* buffer, unsigned int size, DebugInfo* a_di) {
	cout << "Starting disassembler..." << endl;
	if (!(buffer[0] == 'R' && buffer[1] == 'J' && buffer[2] == 'H'
			&& buffer[3] == 'G' && buffer[4] == 'P' && buffer[5] == 'L')) {
		throw new PException("Magic number does not match, invalid bytecode");
	}
	di = a_di;
	di->reset();
	//
	// hexdump
	//
	hexdump(buffer, size);
	i = buffer[6] + buffer[7] * 256;
	cout << "i is now " << i << " size is now " << size << endl;
	for (; i < size;) {
		di->printLine(i);
		cout << "i=" << i << " ";
		char f = buffer[i];
		cout << "f=" << (unsigned int) f << " ";
		unsigned short l = (buffer[i + 1] & 255) + (buffer[i + 2] << 8);
		unsigned short a = (buffer[i + 3] & 255) + (buffer[i + 4] << 8);
		string s = decode(f, l, a);
		cout << s << endl;
	}
}

void Disassembler::hexdump(char* buf, unsigned int buflen) {
	unsigned int i, j;
	for (i = 0; i < buflen; i += 16) {
		printf("%06x: ", i);
		for (j = 0; j < 16; j++)
			if (i + j < buflen)
				printf("%02x ", buf[i + j] &255);
			else
				printf("   ");
		printf(" ");
		for (j = 0; j < 16; j++)
			if (i + j < buflen)
				printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
		printf("\n");
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
		switch (l) {
		case 2: // Int
			i += 5;
			break;
		case 5: // float
			i += 13;
			break;
		case 7: // string
			//
			i += 5 + a;
			break;
		case 6: // boolean
			i += 5;
			break;
		default:
			cout << "unexpected LIT value" << endl;
			i += 5;
			break;
		}
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
		i += 5;
		break;
	case 3:
		sf = "LOD";
		//lod: copy a local variable on top of the stack
		i += 5;
		break;
	case 4:
		// sto: pop a value from the stack and put it in a local variable
		sf = "STO";
		i += 5;
		break;
	case 5:
		//cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		sf = "CAL";
		i += 5;
		break;
	case 6:
		// int:
		sf = "INT";
		i += 5;
		break;
	case 7:
		// jmp
		sf = "JMP";
		i += 5;
		break;
	case 8:
		// jpc
		sf = "JPC";
		i += 5;
		break;
	case 9:
		sf = "PRINT";
		i += 5;
		break;
	case 10:
		sf = "DYNCAL";
		i += 5 + a;
		break;
	case 11:
		sf = "OBJ CREATION";
		i += 5;
		break;
	case 12:
		sf = "METHOD CALL";
		i += 5;
		break;
	default:
		sf = "-----------------------------------+> unexpected F value: "
				+ int(f);
		cout << " F=" << (unsigned int) f << " L=" << l << " A=" << a << endl;
		i += 5;
		break;
	}
	string total = sf + " " + sl + "," + sa;
	return total;
}
