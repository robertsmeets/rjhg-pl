/*
 * Interpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_


#include <vector>
#include <iostream>

using namespace std;

class Interpreter {
	vector<char, allocator<char> > buffer;
	int p;
	int b;
	int t;
	int i;
	vector<unsigned short int> s;


public:
	Interpreter(vector<char>);

	virtual ~Interpreter();
	void start();
	int execute_next();
	int base(int);

};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
