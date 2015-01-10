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
	i = 0;
	t = 0;   // is the top of the stack s
	tr = 0;  // is the top of the stack r
	// b = 1;
	p = 0;
	s = vector<unsigned short int>(500); // value stack
	r = vector<unsigned short int>(500); // return stack

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
	cout << "i = "<<i ;
	unsigned short int f = buffer[i];
	i++;
	//
	// using little endian
	//
	unsigned short int l = buffer[i] + (buffer[i+1] << 8) ;
	i+=2;
	unsigned short int a = buffer[i] + (buffer[i+1] << 8) ;
	i+=2;
	cout <<" f = "<<f<<" l = "<<l<<" a = " << a << endl;
	//
	// opcode definitions
	//
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT" << endl;
		t++;
		cout << "s[" << t << "] = " << a << endl;
		s[t] = a;
		break;
	case 2: // opr
		cout << "OPR" ;
		switch (a) {
		case 0:
			cout << " RET" << endl;
			// return
			p=r[tr];
			cout << "returning to " << p << endl;
			tr--;
			if (t==0)
			{
				cout << "exiting program" << endl;
				// exit
				return -1;
			}
			break;
		case 1:
			cout << " UNARY MINUS" << endl;
			s[t] = -s[t];
			break;
		case 2:
			cout << " PLUS" << endl;
			t = t - 1;
			s[t] = s[t] + s[t + 1];
			break;
		case 3:
			cout << " MINUS" << endl;
			t = t - 1;
			s[t] = s[t] - s[t + 1];
			break;
		case 4:
			cout << " MUL" << endl;
			t = t - 1;
			s[t] = s[t] * s[t + 1];
			break;
		case 5:
			cout << " DIV" << endl;
			t = t - 1;
			s[t] = s[t] / s[t + 1];
			break;
		case 6:
			cout << " MOD" << endl;
			s[t] = (s[t] % 2 == 0);
			break;
		case 8:
			cout << " EQ" << endl;
			t = t - 1;
			s[t] = (s[t] == s[t + 1]);
			break;
		case 9:
			cout << " NE" << endl;
			t = t - 1;
			s[t] = (s[t] != s[t + 1]);
			break;
		case 10:
			cout << " LT" << endl;
			t = t - 1;
			s[t] = (s[t] < s[t + 1]);
			break;
		case 11:
			cout << " GE" << endl;
			t = t - 1;
			s[t] = (s[t] >= s[t + 1]);
			break;
		case 12:
			cout << " GT" << endl;
			t = t - 1;
			s[t] = (s[t] > s[t + 1]);
			break;
		case 13:
			cout << " LE" << endl;
			t = t - 1;
			s[t] = (s[t] <= s[t + 1]);
			break;
		default:
			cout << "unexpected A value: " << a << endl;
			return -1;
			break;
		}
		break;
	case 3:
		cout << "LOD" << endl;
		//lod: copy a local variable on top of the stack
		t++;
		// s[t] = s[base(l) + a];
		break;
	case 4: // sto: pop a value from the stack and put it in a local variable
		cout << "STO" << endl;
		// s[base(l) + a] = s[t];
		cout << "output from sto: " << s[t] << endl;
		t--;
		break;
	case 5: //cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		cout << "CAL" << endl;
		cout << "return address " << p << endl;
		r[tr] = p;
		tr++;
		p = a;
		break;
	case 6: // int:
		cout << "INT" << endl;
		t = t + a;
		break;
	case 7: // jmp
		cout << "JMP" << endl;
		p = a;
		break;
	case 8: // jpc
		cout << "JPC" << endl;
		if (s[t] == 0) {
			p = a;
			t = t - 1;
		}
		break;
	default:
		cout << "unexpected F value: " << f << endl;
		return -1;
		break;
	}
	//
	// print the stack
	//
	cout << "stack: ";
    for (int i = 1;i <= t; i++)
    {
    	cout << s[i] << " ";
    }
    cout << endl;
	return 0;
}

