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
	cout << "i="<< i << ": " ;
	unsigned short int f = buffer[i] & 255;
	i++;
	//
	// using little endian
	//
	// unsigned short int l = (buffer[i] & 255)+ (buffer[i+1] << 8) ;
	i+=2;
	unsigned short int a = (buffer[i] & 255)+ (buffer[i+1] << 8) ;
	i+=2;
	// cout <<" f = "<<f<<" l = "<<l<<" a = " << a << endl;
	//
	// opcode definitions
	//
	switch (f) {
	case 1:   // lit: Literal value, to be pushed on the stack
		cout << "LIT " << a ;
		t++;
		// cout << "s[" << t << "] = " << a << endl;
		s[t] = a;
		break;
	case 2: // opr
		cout << "OPR" ;
		switch (a) {
		case 0:
			cout << " RET" ;
			// return
			p=r[tr];
			// cout << "returning to " << p << endl;
			tr--;
			if (t==0)
			{
				cout << "exiting program" ;
				// exit
				return -1;
			}
			break;
		case 1:
			cout << " UNARY MINUS" ;
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
			cout << " MUL" ;
			t = t - 1;
			s[t] = s[t] * s[t + 1];
			break;
		case 5:
			cout << " DIV" ;
			t = t - 1;
			s[t] = s[t] / s[t + 1];
			break;
		case 6:
			cout << " MOD" ;
			s[t] = (s[t] % 2 == 0);
			break;
		case 8:
			cout << " EQ" << endl;
			t = t - 1;
			s[t] = (s[t] == s[t + 1]);
			break;
		case 9:
			cout << " NE" ;
			t = t - 1;
			s[t] = (s[t] != s[t + 1]);
			break;
		case 10:
			cout << " LT" ;
			t = t - 1;
			s[t] = (s[t] < s[t + 1]);
			break;
		case 11:
			cout << " GE" ;
			t = t - 1;
			s[t] = (s[t] >= s[t + 1]);
			break;
		case 12:
			cout << " GT" ;
			t = t - 1;
			s[t] = (s[t] > s[t + 1]);
			break;
		case 13:
			cout << " LE" ;
			t = t - 1;
			s[t] = (s[t] <= s[t + 1]);
			break;
		default:
			cout << "unexpected A value: " << a ;
			return -1;
			break;
		}
		break;
	case 3:
		cout << "LOD" ;
		//lod: copy a local variable on top of the stack
		t++;
		// s[t] = s[base(l) + a];
		break;
	case 4: // sto: pop a value from the stack and put it in a local variable
		cout << "STO " << s[t];
		// s[base(l) + a] = s[t];
		t--;
		break;
	case 5: //cal:
		// parameters should have already been pushed on the stack
		// push the return address on the return stack
		// call the procedure
		//
		cout << "CAL " << a ;
		// cout << "return address " << p+5 << endl;
		r[tr] = p+5;
		tr++;
		p = a;
		break;
	case 6: // int:
		cout << "INT " << a;
		t = t + a;
		break;
	case 7: // jmp
		cout << "JMP " << a;
		p = a;
		break;
	case 8: // jpc
		cout << "JPC " << a;
		if (s[t] == 0) {
			p = a;
			t = t - 1;
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
    for (int i = 1;i <= t; i++)
    {
    	cout << s[i] << " ";
    }
    cout << endl;
	return 0;
}

