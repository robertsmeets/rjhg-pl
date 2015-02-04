/*
 * CInterpreter.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "CInterpreter.h"

using namespace std;

CInterpreter::CInterpreter(char* a_buffer) {
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
	for (unsigned int i = 0; i < 100000;) {
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
	unsigned short int f = *((char*) buffer + pc) & 255;
	pc++;
	//
	// using little endian
	//
	char* lptr = (char*) buffer + pc;
	unsigned short int l = (*lptr & 255) + (*(lptr + 1) << 8);
	pc += 2;
	lptr = (char*) buffer + pc;
	unsigned short int a = (*lptr & 255) + (*(lptr + 1) << 8);
	pc += 2;
	unsigned short int temp;
	//
	// opcode definitions
	//
	void* ptr;
	double* d;
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT " << l;
		switch (l) {
		case 2: // Int
			s[t] = a;
			t++;
			break;
		case 5: // float
			ptr = hm.allocate(a);
			memcpy(&ptr, buffer + pc, a);
			d = new double();
			memcpy(d, buffer + pc, a);
			cout << "The buffer is located at " << buffer << endl;
			cout << "pc is now " << pc << endl;
			cout << "FOUND A FLOAT with length " << a << " and value " << *d
					<< endl;
			pc += a;
			break;
		case 7: // string
			cout << "FOUND A STRING with length " << a << endl;
			ptr = hm.allocate(a);
			memcpy(&ptr, buffer + pc, a);
			cout << "--- Here is a string [";
			for (unsigned int i = 0; i < a; i++) {
				cout << *(buffer + pc + i);
			}
			cout << "]" << endl;

			pc += a;
			break;
		case 6: // boolean
			s[t] = a;
			t++;
			break;
		default:
			cout << "unexpected LIT value: " << l;
			return -1;
		}
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
			if (l > 0) {
				temp = s[t - 1];
			}
			t = b[tb];
			if (l > 0) {
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
		cout << "INT " << l << "," << a;
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

