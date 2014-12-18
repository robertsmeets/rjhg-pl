/*
 * Interpreter.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "Interpreter.h"
#include <vector>

using namespace std;

Interpreter::Interpreter(vector<char> a_buffer) {
	buffer = a_buffer;
	i = 0;
	t = 0;
	b = 1;
	p = 0;
	s = vector<unsigned short int>(500);

}

Interpreter::~Interpreter() {

}

void Interpreter::start() {

	for (int i = 0; i == 0;) {
		i = execute_next();
	}
}

/*

 p, b, t: integer; {program-, base-, topstack-registers}
 i: instruction; {instruction register}
 s: array [1..stacksize] of integer; {datastore}
 */
int Interpreter::execute_next() {
	// f is the opcode
	// l is the 2nd param
	// a is the 3rd param
	//
	unsigned short int f = buffer[i];
	i++;
	unsigned short int l = (buffer[i] << 8) + buffer[i+1] ;
	i+=2;
	unsigned short int a = (buffer[i] << 8) + buffer[i+1] ;
	i+=2;
	//
	// opcode definitions
	//
	//
	switch (f) {
	case 1:   // lit:
		t++;
		s[t] = a;
		break;
	case 2: // opr
		switch (a) {
		case 0:
			// return
			t = b - 1;
			if (t==0)
			{
				// exit
				return -1;
			}
			p = s[t + 3];
			b = s[t + 2];
			break;
		case 1:
			s[t] = -s[t];
			break;

		case 2:
			t = t - 1;
			s[t] = s[t] + s[t + 1];
			break;

		case 3:
			t = t - 1;
			s[t] = s[t] - s[t + 1];
			break;
		case 4:
			t = t - 1;
			s[t] = s[t] * s[t + 1];
			break;
		case 5:
			t = t - 1;
			s[t] = s[t] / s[t + 1];
			break;
		case 6:
			s[t] = (s[t] % 2 == 0);
			break;
		case 8:
			t = t - 1;
			s[t] = (s[t] == s[t + 1]);
			break;
		case 9:
			t = t - 1;
			s[t] = (s[t] != s[t + 1]);
			break;
		case 10:
			t = t - 1;
			s[t] = (s[t] < s[t + 1]);
			break;
		case 11:
			t = t - 1;
			s[t] = (s[t] >= s[t + 1]);
			break;
		case 12:
			t = t - 1;
			s[t] = (s[t] > s[t + 1]);
			break;
		case 13:
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
		//lod
		t++;
		s[t] = s[base(l) + a];
		break;
	case 4: // sto
		s[base(l) + a] = s[t];
		cout << s[t] << endl;
		t = t - 1;
		break;
	case 5: //cal:
		// generate new block mark
		s[t + 1] = base(l);
		s[t + 2] = b;
		s[t + 3] = p;
		b = t + 1;
		p = a;
		break;
	case 6: // int:
		t = t + a;
		break;
	case 7: // jmp
		p = a;
		break;
	case 8: // jpc
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
	return 0;
}

int Interpreter::base(int l) {
	int b1 = b;	    // find base l levels down
	for (; l > 0; l--) {
		b1 = s[b1];
	}
	return b1;
}
