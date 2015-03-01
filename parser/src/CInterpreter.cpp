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

}

CInterpreter::~CInterpreter() {
}

void CInterpreter::start() {
	cout << "Starting interpreter..." << endl;
	unsigned i = 0;
	for (; !i;) {
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
#ifdef DEBUG
	cout << "pc=" << pc << ": ";
#endif
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
#ifdef DEBUG
	cout << "LIT " << l << "," << a;
#endif
		switch (l) {
		case 2: // Int
			s[t].atype = 2;
			s[t].address = a;
			t++;
			break;
		case 5: // float
			ptr = hm.allocate(a);
			memcpy(ptr, buffer + pc, a);
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
#ifdef DEBUG
			cout << "--- allocated " << (void*) ptr << endl;
#endif
			//
			// copy the string to the heap
			//
			*ptr = a & 255;
			*(ptr + 1) = a >> 8;
			memcpy(ptr + 2, buffer + pc, a);
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
			throw PException("unexpected LIT value");
		}
		break;
	case 2: // opr
#ifdef DEBUG
	cout << "OPR";
#endif

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
#ifdef DEBUG
			cout << " RET";
#endif
			// return
			if (tr <= 0) {
				cout << endl << "Exiting program..." << endl;
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
			if (b[tb] == 0) {
				t = 0;
			} else {
				t = b[tb] - 1;
			}
			if (l > 0) {
				s[t] = temp;
				t++;
			}
			break;
		case 1:
#ifdef DEBUG
			cout << " UNARY MINUS";
#endif
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
#ifdef DEBUG
			cout << " PLUS, MINUS oR MUL";
#endif
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
				unsigned int len1 = ((*ptr1) & 255) + (*(ptr1 + 1) << 8);
				unsigned int len2 = ((*ptr2) & 255) + (*(ptr2 + 1) << 8);
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
				stack_element* fr3 = new stack_element();
				fr3->atype = 5;
				fr3->address = hm.allocate(8) - hm.getStart();
				memcpy(st + fr3->address, &d3, 8);
				s[t - 1] = *fr3;
			} else if ((fr1.atype == 5) && (fr2.atype == 2)) {
				//
				// float plus integer
				//
				char* st = hm.getStart();
				memcpy(&d1, st + fr1.address, 8);
				adiptr = (diptr) (fptrs[a][fr1.atype][fr2.atype]);
				d3 = (*adiptr)(d1, fr2.address);
				stack_element* fr3 = new stack_element();
				fr3->atype = 5;
				fr3->address = hm.allocate(8) - hm.getStart();
				memcpy(st + fr3->address, &d3, 8);
				s[t - 1] = *fr3;

			} else if ((fr1.atype == 5) && (fr1.atype == 5)) {
				//
				// both floats
				//
				char* st = hm.getStart();
				//
				// copy both floats to temp variables d1 and d2
				//
				memcpy(&d1, st + fr1.address, 8);
				memcpy(&d2, st + fr2.address, 8);
				//
				// find the function
				//
				addptr = (ddptr) (fptrs[a][fr1.atype][fr2.atype]);
				//
				// perform the operation
				//
				d3 = (*addptr)(d1, d2);
				//
				//
				stack_element* fr3 = new stack_element();
				fr3->atype = 5;
				fr3->address = hm.allocate(8) - hm.getStart();
				memcpy(st + fr3->address, &d3, 8);
				s[t - 1] = *fr3;
			} else {
				ostringstream oss;
				oss << "operation " << a << ": incompatible types " << fr1.atype
						<< " and " << fr2.atype;
				throw PException(oss.str());
			}
			break;
		case 5:
#ifdef DEBUG
			cout << " DIV";
#endif
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
#ifdef DEBUG
			cout << " MOD";
#endif
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
				abiiptr = (biiptr) fptrs[a][fr1.atype][fr2.atype];
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
				ostringstream oss;
				oss << "operation " << a << ": incompatible types " << fr1.atype
						<< " and " << fr2.atype;
				throw PException(oss.str());
			}

			break;

		default:
			throw PException("unexpected A value");
			return -1;
			break;
		}
		break;
	case 3:
#ifdef DEBUG
		cout << "LOD " << a << " ";
#endif
		//
		// lod: copy a local variable or parameter on top of the stack
		//
		s[t] = s[b[tb - 1] + a];
		t++;
		break;
	case 4:	// sto: pop a value from the stack and put it in a local variable or parameter
#ifdef DEBUG
	cout << "STO " << a << " ";
#endif
		t--;
		s[b[tb - 1] + a] = s[t];
		break;
	case 5:			//cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
#ifdef DEBUG
		cout << "CAL " << a;
#endif

		r[tr] = pc;
		tr++;
		pc = a;
		break;
	case 6:			// int:
#ifdef DEBUG
	cout << "INT " << l << "," << a;
#endif
		//
		// this creates a new block with depth a for local variables and parameters
		//
		b[tb] = t - l;
		tb++;
		//
		// l contains the number of parameters
		// a contains the number of local variables
		//
		// only add a because the parameters will be pushed on the stack
		//
		t += a;
		break;
	case 7:			// jmp
#ifdef DEBUG
	cout << "JMP " << a;
#endif
		pc = a;
		break;
	case 8:			// jpc - jump when false
#ifdef DEBUG
	cout << "JPC " << a;
#endif
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
#ifdef DEBUG
	cout << "PRINT " << a;
#endif
		t--;
		fr1 = s[t];
		if (fr1.atype == 7) {
			char* ptr = hm.getStart() + fr1.address;
			print_a_string(ptr);
		} else if (fr1.atype == 5) {
			//
			// float
			//
			char* ptr = hm.getStart() + fr1.address;
			memcpy(&d1, ptr, 8);
			cout << d1 << endl;
		} else if (fr1.atype == 2) {
			cout << fr1.address << endl;
		}
		break;

	case 10: // external function call
#ifdef DEBUG
	cout << "EXTCALL " << a;
#endif
		// parameters should have already been pushed on the stack
		//
		// first get the string that is the procedure name
		// get some memory
		//
		ptr = (char*) malloc(255);
		//
		// copy the string to a local variable
		//
		memcpy(ptr, buffer + pc, a);
		ptr[a] = '\0';
		pc += a;
		//
		// call the procedure
		//
		call_external(ptr, l);
		free(ptr);
		break;

	default:
		throw PException("unexpected F value");
		break;
	}
	//
	// print the stack
	//
#ifdef DEBUG
	cout << "      stack: ";
	for (unsigned int i = 0; i < t; i++) {
		char* adr;
		//cout << s[i].atype << ":";
		cout << "[";
		switch (s[i].atype) {
			case 2:
			cout << s[i].address;
			break;
			case 5:
			adr = hm.getStart() + s[i].address;
			double d;
			memcpy(&d, adr, 8);
			cout << d;
			break;
			default:
			cout << "?" << s[i].atype;
			break;
		}
		cout << "]";
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
#endif
	return 0;
}

void CInterpreter::print_a_string(char* ptr) {
	unsigned int len = ((*ptr) & 255) + (*(ptr + 1) << 8);
	print_a_string(ptr + 2, len);
	cout << endl;

}

void CInterpreter::print_a_string(char* ptr, unsigned int len) {
	for (char* i = ptr; i < ptr + len; i++) {
		cout << *i;
	}
}

void CInterpreter::call_external(char* function_name,
		unsigned int nparameters) {
	string libpath = "msvcrt.dll";
	const char* lp = libpath.c_str();
	DLLib* ll = dlLoadLibrary(lp);
	if (ll == NULL) {
		throw PException("could not find library " + libpath);
	}
	//
	// dlLoadLibrary loads a dynamic library at libpath and returns a handle
	// to it for use in dlFreeLibrary and dlFindSymbol calls.
	// dlFreeLibrary frees the loaded library with handle pLib.
	//
	DCpointer sym = dlFindSymbol(ll, function_name);
	if (sym == NULL) {
		throw PException("could not find external symbol");
	}
	DCCallVM* vm = dcNewCallVM(4096);
	dcMode(vm, DC_CALL_C_DEFAULT);
	dcReset(vm);
	//
	// get the arguments
	//
	stack_element f = s[t - 1];
	unsigned int atype = f.atype;
	double arg_in;
	char* adr;
	char* str;
	unsigned int len;
	switch (atype) {
	case 5: // double
		adr = hm.getStart() + f.address;
		memcpy(&arg_in, adr, 8);
		dcArgDouble(vm, arg_in);
		break;
	case 7: // string
		adr = hm.getStart() + f.address;
		memcpy(&arg_in, adr, 8);
		len = ((*adr) & 255) + (*(adr + 1) << 8);
		str = (char*) malloc(len + 1);
		memcpy(str, adr + 2, len);
		str[len] = '\0';
		dcArgChar(vm, *str);
		free(str);
		break;
	default:
		throw PException("unexpected type in external call");
	}
	double r = dcCallDouble(vm, sym);
	// dcCallPointer(vm,sym);
	dcFree(vm);
	dlFreeLibrary(ll);
	//
	// put the result on the stack
	//
	char* ptr = hm.allocate(8);
	memcpy(ptr, &r, 8);
	t = b[tb] + 2;
	tb--;
	s[t].atype = 5;
	s[t].address = (short unsigned int) (ptr - hm.getStart());
	t++;
}
