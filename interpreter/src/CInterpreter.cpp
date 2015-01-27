/*
 * CInterpreter.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "CInterpreter.h"

#include <vector>

using namespace std;

CInterpreter::CInterpreter(vector<char> a_buffer) {
	buffer = a_buffer;
	t = 0;   // is the top of the stack s
	tr = 0;  // is the top of the stack r
	pc = 0; // program counter
	tb = 0;
	s = vector<unsigned short int>(500); // value stack
	r = vector<unsigned short int>(500); // return stack
	b = vector<unsigned short int>(500); // block address stack
}

CInterpreter::~CInterpreter() {

}

void CInterpreter::start() {
	for (unsigned int i = 0; i < buffer.size();) {
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
	unsigned short int f = buffer[pc] & 255;
	pc++;
	//
	// using little endian
	//
	pc += 2;
	unsigned short int a = (buffer[pc] & 255) + (buffer[pc + 1] << 8);
	pc += 2;
	// cout <<" f = "<<f<<" l = "<<l<<" a = " << a << endl;
	//
	// opcode definitions
	//
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT " << a;
		t++;
		// cout << "s[" << t << "] = " << a << endl;
		s[t] = a;
		break;
	case 2: // opr
		cout << "OPR";
		switch (a) {
		case 0:
			cout << " RET";
			// return
			pc = r[tr];
			if (pc == 0) {
				cout << "exiting program" << endl;;
				// exit
				return -1;
			}
			tr--;
			//
			// also free the block
			//
			t = b[tb];
			tb--;
			break;
		case 1:
			cout << " UNARY MINUS";
			s[t] = -s[t];
			break;
		case 2:
			cout << " PLUS";
			t = t - 1;
			s[t] = s[t] + s[t + 1];
			break;
		case 3:
			cout << " MINUS";
			t = t - 1;
			s[t] = s[t] - s[t + 1];
			break;
		case 4:
			cout << " MUL";
			t = t - 1;
			s[t] = s[t] * s[t + 1];
			break;
		case 5:
			cout << " DIV";
			t = t - 1;
			s[t] = s[t] / s[t + 1];
			break;
		case 6:
			cout << " MOD";
			s[t] = (s[t] % 2 == 0);
			break;
		case 8:
			cout << " EQ" << endl;
			t = t - 1;
			s[t] = (s[t] == s[t + 1]);
			break;
		case 9:
			cout << " NE";
			t = t - 1;
			s[t] = (s[t] != s[t + 1]);
			break;
		case 10:
			cout << " LT";
			t = t - 1;
			s[t] = (s[t] < s[t + 1]);
			break;
		case 11:
			cout << " GE";
			t = t - 1;
			s[t] = (s[t] >= s[t + 1]);
			break;
		case 12:
			cout << " GT";
			t = t - 1;
			s[t] = (s[t] > s[t + 1]);
			break;
		case 13:
			cout << " LE";
			t = t - 1;
			s[t] = (s[t] <= s[t + 1]);
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
		t++;
		s[t] = s[b[tb] + a];
		break;
	case 4: // sto: pop a value from the stack and put it in a local variable or parameter
		cout << "STO " << s[t];
		s[b[tb] + a] = s[t];
		t--;
		break;
	case 5: //cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		cout << "CAL " << a;
		r[tr] = pc + 5;
		tr++;
		pc = a;
		break;
	case 6: // int:
		cout << "INT " << a;
		//
		// this creates a new block with depth a for local variables and parameters
		//
		tb++;
		b[tb] = t -1;
		t = t + a;
		break;
	case 7: // jmp
		cout << "JMP " << a;
		pc = a;
		break;
	case 8: // jpc
		cout << "JPC " << a;
		if (s[t] == 0) {
			pc = a;
			t = t - 1;
		}
		break;
	case 9: // print
		cout << "PRINT " << s[t];
		t = t - 1;
		break;
	case 10: // minint
		cout << "MININT " << a;
		if (t < a) {
			t = 0;
		} else {
			t = t - a;
		}
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
	for (unsigned int i = 1; i <= t; i++) {
		cout << s[i] << " ";
	}

	cout << "      bstack: ";
		for (unsigned int i = 1; i <= tb; i++) {
			cout << b[i] << " ";
		}

	cout << endl;
	return 0;
}

