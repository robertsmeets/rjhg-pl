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
#include <cstdlib>
#include <cstring>

#include "HeapManager.h"
<<<<<<< HEAD
#include "PException.h"
#include "CFunctions.h"
=======
>>>>>>> Float, String literals now work

using namespace std;

class CInterpreter {
<<<<<<< HEAD
	struct stack_element {
	  unsigned short int atype;
	  unsigned short int address;
	} ;
=======
>>>>>>> Float, String literals now work
	char* buffer;
	unsigned int pc;
	unsigned int t;
	unsigned int tr;
	unsigned int tb;
	vector<stack_element> s;
	vector<unsigned short int> r;
	vector<unsigned short int> b;
	HeapManager hm;
<<<<<<< HEAD
=======

>>>>>>> Float, String literals now work
public:
	CInterpreter(char*);
	virtual ~CInterpreter();
	void start();
	int execute_next();
	void print_a_string(char*);
	void print_a_string(char* , unsigned int );
};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
