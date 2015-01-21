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
	int p;
//	int b;
	int t;
	int tr;
	int i;
	vector<unsigned short int> s;
	vector<unsigned short int> r;

public:
	CInterpreter(vector<char>);
	virtual ~CInterpreter();
	void start();
	int execute_next();
	int base(int);

};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
