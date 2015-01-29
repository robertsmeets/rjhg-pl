/*
 * CInterpreter.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "CInterpreter.h"

#include <vector>

using namespace std;

CInterpreter::CInterpreter(vector<unsigned char>* a_buffer) {
	buffer = a_buffer;
	pc = 0; // program counter
	t = 0;   // is the top of the stack s
	tr = 0;  // is the top of the stack r
	tb = 0;  // is the top of the stack b
	s = vector<unsigned short int>(500); // value stack
	r = vector<unsigned short int>(500); // return stack
	b = vector<unsigned short int>(500); // block address stack
}

CInterpreter::~CInterpreter() {

}

void CInterpreter::start() {
	for (unsigned int i = 0; i < buffer->size();) {
		i = execute_next();
	}
}

/*

 p, b, t: integer; {program-, base-, topstack-registers}
 i: instruction; {instruction register}
 s: array [1..stacksize] of integer; {datastore}
 */
int CInterpreter::execute_next() {
	//
	// f is the opcode
	// l is the 2nd param
	// a is the 3rd param
	//
	cout << "pc=" << pc << ": ";
	unsigned short int f = buffer->at(pc) & 255;
	pc++;
	//
	// using little endian
	//
	unsigned short int l = (buffer->at(pc) & 255) + (buffer->at(pc + 1) << 8);
	pc += 2;
	unsigned short int a = (buffer->at(pc) & 255) + (buffer->at(pc + 1) << 8);
	pc += 2;
	unsigned short int temp;
	//
	// opcode definitions
	//
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT " << a;
		s[t] = a;
		t++;
		break;
	case 2: // opr
		cout << "OPR";
		switch (a) {
		case 0:
			cout << " RET";
			// return
			if (tr <= 0) {
				cout << endl << "exiting program" << endl;
				// exit
				return -1;
			}
			tr--;
			pc = r[tr];
			//
			// also free the block
			//
			// there may be a return value on the top of the stack that needs to be saved
			//
			tb--;
			if (l > 0)
			{
				temp = s[t-1];
			}
			t = b[tb];
			if (l > 0)
			{
				s[t] = temp;
				t++;
			}
			break;
		case 1:
			cout << " UNARY MINUS";
			s[t - 1] = -s[t - 1];
			break;
		case 2:
			cout << " PLUS";
			t--;
			s[t - 1] = s[t] + s[t - 1];
			break;
		case 3:
			cout << " MINUS";
			t--;
			s[t - 1] = s[t - 1] - s[t];
			break;
		case 4:
			cout << " MUL";
			t--;
			s[t - 1] = s[t] * s[t - 1];
			break;
		case 5:
			cout << " DIV";
			t--;
			s[t - 1] = s[t - 1] / s[t];
			break;
		case 6:
			cout << " MOD";
			t--;
			s[t - 1] = s[t - 1] % s[t];
			break;
		case 8:
			cout << " EQ" << endl;
			t--;
			s[t - 1] = (s[t - 1] == s[t]);
			break;
		case 9:
			cout << " NE";
			t--;
			s[t - 1] = (s[t - 1] != s[t]);
			break;
		case 10:
			cout << " LT";
			t--;
			s[t - 1] = (s[t - 1] < s[t]);
			break;
		case 11:
			cout << " GE";
			t--;
			s[t - 1] = (s[t - 1] >= s[t]);
			break;
		case 12:
			cout << " GT";
			t--;
			s[t - 1] = (s[t - 1] > s[t]);
			break;
		case 13:
			cout << " LE";
			t--;
			s[t - 1] = (s[t - 1] <= s[t]);
			break;
		default:
			cout << "unexpected A value: " << a;
			return -1;
			break;
		}
		break;
	case 3:
		cout << "LOD " << a << " ";
		//
		// lod: copy a local variable or parameter on top of the stack
		//
		s[t] = s[b[tb - 1] + a];
		t++;
		break;
	case 4: // sto: pop a value from the stack and put it in a local variable or parameter
		cout << "STO " << a << " ";
		t--;
		s[b[tb - 1] + a] = s[t];
		break;
	case 5: //cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		cout << "CAL " << a;
		r[tr] = pc;
		tr++;
		pc = a;
		break;
	case 6: // int:
		cout << "INT " << l << ","<< a;
		//
		// this creates a new block with depth a for local variables and parameters
		//
		b[tb] = t - l;
		tb++;
		t = t + a;
		break;
	case 7: // jmp
		cout << "JMP " << a;
		pc = a;
		break;
	case 8: // jpc
		cout << "JPC " << a;
		if (s[t - 1] == 0) {
			pc = a;
		}
		t--;
		break;
	case 9: // print
		t--;
		cout << "PRINT " << s[t];
		break;
	default:
		cout << "unexpected F value: " << f;
		return -1;
		break;
	}
	//
	// print the stack
	//
	cout << "      stack: ";
	for (unsigned int i = 0; i < t; i++) {
		cout << s[i] << " ";
	}

	cout << "      bstack: ";
	for (unsigned int i = 0; i < tb; i++) {
		cout << b[i] << " ";
	}

	cout << "      rstack: ";
	for (unsigned int i = 0; i < tr; i++) {
		cout << r[i] << " ";
	}

	cout << endl;
	return 0;
}

