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
	s = vector<stack_element>(500);
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
	stack_element temp;
	//
	// opcode definitions
	//
	char* ptr;
	double d1;
	double d2;
	double d3;
	stack_element fr1;
	stack_element fr2;
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT " << l << "," << a;
		switch (l) {
		case 2: // Int
			s[t].atype = 2;
			s[t].address = a;
			t++;
			break;
		case 5: // float
			ptr = hm.allocate(a);
			memcpy(ptr, buffer + pc, a);
			memcpy(&d1, buffer + pc, a);
			cout << endl;
			cout << "The buffer is located at " << (void*) buffer << endl;
			cout << "pc is now " << pc << endl;
			cout << "FOUND A FLOAT with length " << a << " and value " << d1
					<< endl;
			//
			// put the pointer and the type on the stack
			//
			s[t].atype = 5;
			s[t].address = (short unsigned int) (ptr - hm.getStart());
			t++;
			pc += a;
			break;
		case 7: // string
			//
			// get some memory
			//
			ptr = hm.allocate(a + 2);
			cout << "--- allocated " << (void*) ptr << endl;
			//
			// copy the string to the heap
			//
			*ptr = a & 255;
			*(ptr + 1) = a >> 8;
			cout << "--- copying a string from " << (void*) (buffer + pc)
					<< " to " << (void*) (ptr + 2) << " with length " << a
					<< endl;
			cout << "--- original" << endl;
			print_a_string(buffer + pc, a);
			memcpy(ptr + 2, buffer + pc, a);
			cout << "--- copy" << endl;
			print_a_string(ptr);
			//
			// put the pointer and the type on the stack
			//
			s[t].atype = 7;
			s[t].address = (short unsigned int) (ptr - hm.getStart());
			t++;
			pc += a;
			break;
		case 6: // boolean
			s[t].atype = 6;
			s[t].address = a;
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
			fr1 = s[t - 1];
			if (fr1.atype != 2) {
				throw PException("type must be integer");
			}
			fr1.address = -fr1.address;
			s[t - 1] = fr1;
			break;
		case 2:
			cout << " PLUS";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype == 2) && (fr2.atype == 2)) {
				//
				// add two integers
				//
				fr1.atype = 2;
				fr1.address = fr1.address + fr2.address;
				s[t - 1] = fr1;
			} else if ((fr1.atype == 7) && (fr2.atype == 7)) {
				//
				// add two strings
				//
				char * ptr1 = hm.getStart() + fr1.address;
				char * ptr2 = hm.getStart() + fr2.address;
				unsigned int len1 = *ptr1 + (*(ptr1 + 1) >> 8);
				unsigned int len2 = *ptr2 + (*(ptr2 + 1) >> 8);
				unsigned int newlen = len1 + len2;
				ptr = hm.allocate(newlen + 2);
				*ptr = newlen & 255;
				*(ptr + 1) = newlen >> 8;
				memcpy(ptr + 2, ptr1 + 2, len1);
				memcpy(ptr + len1 + 2, ptr2 + 2, len2);
				fr1.address = (unsigned int) (ptr - hm.getStart());
				fr1.atype = 7;
				s[t - 1] = fr1;
			} else if ((fr1.atype == 2) && (fr2.atype == 5)) { // integer plus float
				cout << "------------------INT plus FLOAT" << endl;
				fr1.atype = 5;
				memcpy(&d2, hm.getStart() + fr2.address, 8);
				d3 = fr1.address + d2;
				memcpy(hm.getStart() + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			}
			else if ((fr1.atype == 5) && (fr2.atype == 2)) { // float plus integer
				cout << "------------------ FLOAT plus INT" << endl;
				memcpy(&d1, hm.getStart() + fr1.address, 8);
				d3 = d1 + fr2.address;
				memcpy(hm.getStart() + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			}
			else if ((fr1.atype == 5) && (fr1.atype == 5)) { // both floats
				cout << "------------------ FLOAT plus FLOAT" << endl;
				memcpy(&d1, hm.getStart() + fr1.address, 8);
				memcpy(&d2, hm.getStart() + fr2.address, 8);
				d3 = d1 + d2;
				memcpy(hm.getStart() + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			} else {
				throw PException(
						"plus: both types must be integer or float or string or something");
			}
			break;
		case 3:
			cout << " MINUS";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 2;
			fr1.address = fr1.address - fr2.address;
			s[t - 1] = fr1;
			break;
		case 4:
			cout << " MUL";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 2;
			fr1.address = fr1.address * fr2.address;
			s[t - 1] = fr1;
			break;
		case 5:
			cout << " DIV";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 2;
			fr1.address = fr1.address / fr2.address;
			s[t - 1] = fr1;
			break;
		case 6:
			cout << " MOD";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 2;
			fr1.address = fr1.address % fr2.address;
			s[t - 1] = fr1;
			break;
		case 8:
			cout << " EQ" << endl;
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address == fr2.address;
			s[t - 1] = fr1;
			break;
		case 9:
			cout << " NE";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address != fr2.address;
			s[t - 1] = fr1;
			break;
		case 10:
			cout << " LT";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address < fr2.address;
			s[t - 1] = fr1;
			break;
		case 11:
			cout << " GE";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address >= fr2.address;
			s[t - 1] = fr1;
			break;
		case 12:
			cout << " GT";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address > fr2.address;
			s[t - 1] = fr1;

			break;
		case 13:
			cout << " LE";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("comparison both types must be integer");
			}
			fr1.atype = 6;
			fr1.address = fr1.address <= fr2.address;
			s[t - 1] = fr1;
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
		b[tb] = t;
		tb++;
		//
		// a contains the number of local variables
		// l contains the number of parameters
		//
		// only add a because the parameters will be pushed on the stack
		//
		t += a;
		break;
	case 7: // jmp
		cout << "JMP " << a;
		pc = a;
		break;
	case 8: // jpc
		cout << "JPC " << a;
		fr1 = s[t - 1];
		if (fr1.atype != 6) {
			throw PException("JPC value is not boolean");
		}
		if (fr1.address == 0) {
			pc = a;
		}
		t--;
		break;
	case 9: // print
		t--;
		fr1 = s[t];
		cout << "PRINT type =" << fr1.atype << " address = " << fr1.address
				<< endl;
		if (fr1.atype == 7) {
			char* ptr = hm.getStart() + fr1.address;
			print_a_string(ptr);

		} else if (fr1.atype == 5) {
			//
			// float
			//
			char* ptr = hm.getStart() + fr1.address;
			memcpy(&d1, ptr, 8);
			cout << "double with value " << d1 << endl;
		}
		break;
	default:
		throw PException("unexpected F value");
		break;
	}
	//
	// print the stack
	//
	cout << "      stack: ";
	for (unsigned int i = 0; i < t; i++) {
		cout << s[i].atype << ":" << s[i].address << " ";
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

void CInterpreter::print_a_string(char* ptr) {
	unsigned int len = (unsigned int) (*ptr + (*(ptr + 1) >> 8));
	print_a_string(ptr + 2, len);
}

void CInterpreter::print_a_string(char* ptr, unsigned int len) {
	cout << "[";
	for (char* i = ptr; i < ptr + len; i++) {
		cout << *i;
	}
	cout << "]" << endl;
}
