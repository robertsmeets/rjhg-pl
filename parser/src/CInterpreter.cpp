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

unsigned int func_plus_ii(unsigned int i, unsigned int j) {
	return i + j;
}

double func_plus_id(int i, double j) {
	return i + j;
}

double func_plus_di(double i, int j) {
	return i + j;
}

double func_plus_dd(double i, double j) {
	return i + j;
}

unsigned int func_minus_ii(unsigned int i, unsigned int j) {
	return i - j;
}

double func_minus_id(int i, double j) {
	return i - j;
}

double func_minus_di(double i, int j) {
	return i - j;
}

double func_minus_dd(double i, double j) {
	return i - j;
}

unsigned int func_mul_ii(unsigned int i, unsigned int j) {
	return i * j;
}

double func_mul_id(int i, double j) {
	return i * j;
}

double func_mul_di(double i, int j) {
	return i * j;
}

double func_mul_dd(double i, double j) {
	return i * j;
}

bool func_eq_ii(unsigned int i, unsigned int j) {
	return i == j;
}

bool func_eq_id(int i, double j) {
	return i == j;
}

bool func_eq_di(double i, int j) {
	return i == j;
}

bool func_eq_dd(double i, double j) {
	return i == j;
}

bool func_ne_ii(unsigned int i, unsigned int j) {
	return i != j;
}

bool func_ne_id(unsigned int i, double j) {
	return i != j;
}

bool func_ne_di(double i, unsigned int j) {
	return i != j;
}

bool func_ne_dd(double i, double j) {
	return i != j;
}

bool func_lt_ii(unsigned int i, unsigned int j) {
	return i < j;
}

bool func_lt_id(unsigned int i, double j) {
	return i < j;
}

bool func_lt_di(double i, int j) {
	return i < j;
}

bool func_lt_dd(double i, double j) {
	return i < j;
}

bool func_ge_ii(unsigned int i, unsigned int j) {
	return i >= j;
}

bool func_ge_id(unsigned int i, double j) {
	return i >= j;
}

bool func_ge_di(double i, unsigned int j) {
	return i >= j;
}

bool func_ge_dd(double i, double j) {
	return i >= j;
}

bool func_gt_ii(unsigned int i, unsigned int j) {
	return i > j;
}

bool func_gt_id(unsigned int i, double j) {
	return i > j;
}

bool func_gt_di(double i, unsigned int j) {
	return i > j;
}

bool func_gt_dd(double i, double j) {
	return i > j;
}

bool func_le_ii(unsigned int i, unsigned int j) {
	return i <= j;
}

bool func_le_id(unsigned int i, double j) {
	return i <= j;
}

bool func_le_di(double i, unsigned int j) {
	return i <= j;
}

bool func_le_dd(double i, double j) {
	return i <= j;
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
		//
		// set up a call table matrix
		// entries; first the operator 0..13
		// second the first argument type 1..7
		// third the second argument type 1..7
		//
		// The result is a function
		//
		//int (*foo)(int, int);
		//		foo = (int (*)(int, int))(&func_plus_ii);}

		typedef unsigned int (*iiptr)(unsigned int, unsigned int);
		typedef double (*ddptr)(double, double);
		typedef double (*idptr)(unsigned int, double);
		typedef double (*diptr)(double, unsigned int);

		typedef bool (*biiptr)(unsigned int, unsigned int);
		typedef bool (*bddptr)(double, double);
		typedef bool (*bidptr)(unsigned int, double);
		typedef bool (*bdiptr)(double, unsigned int);

		iiptr fptrs[14][8][8];
		//
		// fill up for OPR 2 (PLUS);
		//
		fptrs[2][2][2] = (iiptr) (&func_plus_ii);
		fptrs[2][2][5] = (iiptr) (&func_plus_id);
		fptrs[2][5][2] = (iiptr) (&func_plus_di);
		fptrs[2][5][5] = (iiptr) (&func_plus_dd);
		//
		// fill up for OPR 3 (MINUS);
		//
		fptrs[3][2][2] = (iiptr) (&func_minus_ii);
		fptrs[3][2][5] = (iiptr) (&func_minus_id);
		fptrs[3][5][2] = (iiptr) (&func_minus_di);
		fptrs[3][5][5] = (iiptr) (&func_minus_dd);
		//
		// fill up for OPR 4 (MUL);
		//
		fptrs[4][2][2] = (iiptr) (&func_mul_ii);
		fptrs[4][2][5] = (iiptr) (&func_mul_id);
		fptrs[4][5][2] = (iiptr) (&func_mul_di);
		fptrs[4][5][5] = (iiptr) (&func_mul_dd);
		//
		// fill up for OPR 8..13
		//
		fptrs[8][2][2] = (iiptr) (&func_eq_ii);
		fptrs[8][2][5] = (iiptr) (&func_eq_id);
		fptrs[8][5][2] = (iiptr) (&func_eq_di);
		fptrs[8][5][5] = (iiptr) (&func_eq_dd);

		fptrs[9][2][2] = (iiptr) (&func_ne_ii);
		fptrs[9][2][5] = (iiptr) (&func_ne_id);
		fptrs[9][5][2] = (iiptr) (&func_ne_di);
		fptrs[9][5][5] = (iiptr) (&func_ne_dd);

		fptrs[10][2][2] = (iiptr) (&func_lt_ii);
		fptrs[10][2][5] = (iiptr) (&func_lt_id);
		fptrs[10][5][2] = (iiptr) (&func_lt_di);
		fptrs[10][5][5] = (iiptr) (&func_lt_dd);

		fptrs[11][2][2] = (iiptr) (&func_ge_ii);
		fptrs[11][2][5] = (iiptr) (&func_ge_id);
		fptrs[11][5][2] = (iiptr) (&func_ge_di);
		fptrs[11][5][5] = (iiptr) (&func_ge_dd);

		fptrs[12][2][2] = (iiptr) (&func_gt_ii);
		fptrs[12][2][5] = (iiptr) (&func_gt_id);
		fptrs[12][5][2] = (iiptr) (&func_gt_di);
		fptrs[12][5][5] = (iiptr) (&func_gt_dd);

		fptrs[13][2][2] = (iiptr) (&func_le_ii);
		fptrs[13][2][5] = (iiptr) (&func_le_id);
		fptrs[13][5][2] = (iiptr) (&func_le_di);
		fptrs[13][5][5] = (iiptr) (&func_le_dd);

		iiptr aiiptr;
		idptr aidptr;
		diptr adiptr;
		ddptr addptr;

		biiptr abiiptr;
		bidptr abidptr;
		bdiptr abdiptr;
		bddptr abddptr;

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
		case 3:
		case 4:
			cout << " PLUS, MINUS oR MUL";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype == 2) && (fr2.atype == 2)) {
				//
				// operation on two integers
				//
				aiiptr = fptrs[a][fr1.atype][fr2.atype];
				fr1.address = (*aiiptr)(fr1.address, fr2.address);
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
				s[t - 1] = fr1;
			} else if ((fr1.atype == 2) && (fr2.atype == 5)) {
				//
				// integer plus float
				//
				char* st = hm.getStart();
				memcpy(&d2, st + fr2.address, 8);
				aidptr = (idptr) (fptrs[a][fr1.atype][fr2.atype]);
				d3 = (*aidptr)(fr1.address, d2);
				fr1.atype = 5;
				memcpy(st + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			} else if ((fr1.atype == 5) && (fr2.atype == 2)) {
				//
				// float plus integer
				//
				char* st = hm.getStart();
				memcpy(&d1, st + fr1.address, 8);
				adiptr = (diptr) (fptrs[a][fr1.atype][fr2.atype]);
				d3 = (*adiptr)(d1, fr2.address);
				memcpy(st + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			} else if ((fr1.atype == 5) && (fr1.atype == 5)) {
				//
				// both floats
				//
				char* st = hm.getStart();
				memcpy(&d1, st + fr1.address, 8);
				memcpy(&d2, st + fr2.address, 8);
				addptr = (ddptr) (fptrs[a][fr1.atype][fr2.atype]);
				d3 = (*addptr)(d1, d2);
				memcpy(st + fr1.address, &d3, 8);
				s[t - 1] = fr1;
			} else {
				throw PException("plus: incompatible types");
			}
			break;
		case 5:
			cout << " DIV";
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype != 2) || (fr2.atype != 2)) {
				throw PException("division both types must be integer");
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
				throw PException("modulo both types must be integer");
			}
			fr1.atype = 2;
			fr1.address = fr1.address % fr2.address;
			s[t - 1] = fr1;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			t--;
			fr1 = s[t - 1];
			fr2 = s[t];
			if ((fr1.atype == 2) && (fr2.atype == 2)) {
				//
				// operation on two integers
				//
				abiiptr = (biiptr)fptrs[a][fr1.atype][fr2.atype];
				bool eq = (*abiiptr)(fr1.address, fr2.address);
				fr1.atype = 6;
				fr1.address = eq;
				s[t - 1] = fr1;
			} else if ((fr1.atype == 2) && (fr2.atype == 5)) {
				//
				// integer plus float
				//
				char* st = hm.getStart();
				memcpy(&d2, st + fr2.address, 8);
				abidptr = (bidptr) (fptrs[a][fr1.atype][fr2.atype]);
				bool eq = (*abidptr)(fr1.address, d2);
				fr1.atype = 6;
				fr1.address = eq;
				s[t - 1] = fr1;
			} else if ((fr1.atype == 5) && (fr2.atype == 2)) {
				//
				// float plus integer
				//
				char* st = hm.getStart();
				memcpy(&d1, st + fr1.address, 8);
				abdiptr = (bdiptr) (fptrs[a][fr1.atype][fr2.atype]);
				bool eq = (*abdiptr)(d1, fr2.address);
				fr1.atype = 6;
				fr1.address = eq;
				s[t - 1] = fr1;
			} else if ((fr1.atype == 5) && (fr1.atype == 5)) {
				//
				// both floats
				//
				char* st = hm.getStart();
				memcpy(&d1, st + fr1.address, 8);
				memcpy(&d2, st + fr2.address, 8);
				abddptr = (bddptr) (fptrs[a][fr1.atype][fr2.atype]);
				bool eq = (*abddptr)(d1, d2);
				fr1.atype = 6;
				fr1.address = eq;
				s[t - 1] = fr1;
			} else {
				throw PException("plus: incompatible types");
			}

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
	case 4:	// sto: pop a value from the stack and put it in a local variable or parameter
		cout << "STO " << a << " ";
		t--;
		s[b[tb - 1] + a] = s[t];
		break;
	case 5:			//cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		cout << "CAL " << a;
		r[tr] = pc;
		tr++;
		pc = a;
		break;
	case 6:			// int:
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
	case 7:			// jmp
		cout << "JMP " << a;
		pc = a;
		break;
	case 8:			// jpc
		cout << "JPC " << a;
		fr1 = s[t - 1];
		if (fr1.atype != 6) {
			throw PException("JPC value is not boolean");
		}
		if (fr1.address != 0) {
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
