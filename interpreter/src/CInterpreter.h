/*
 * CInterpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_

#include <vector>
#include <iostream>

using namespace std;

class CInterpreter {
	vector<char, allocator<char> > buffer;
	unsigned int pc;
	unsigned int t;
	unsigned int tr;
	unsigned int tb;
	vector<unsigned short int> s;
	vector<unsigned short int> r;
	vector<unsigned short int> b;

public:
	CInterpreter(vector<char>);
	virtual ~CInterpreter();
	void start();
	int execute_next();

};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
