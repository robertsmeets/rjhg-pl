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
#include <sstream>
#include <dyncall.h>
#include <dyncall_callf.h>
#include <dynload.h>
#include <math.h>

#include "HeapManager.h"
#include "PException.h"
#include "CFunctions.h"
#include "DebugInfo.h"

using namespace std;

class HeapManager; // forward declaration

struct stack_element {
		unsigned short int atype;
		unsigned short int address;
	};

class CInterpreter {
	DebugInfo* di;
	char* buffer;
	unsigned int pc;
	unsigned int t;
	unsigned int tr;
	unsigned int tb;
	vector<stack_element> s;
	vector<unsigned short int> r;
	vector<unsigned short int> b;
	HeapManager* hm;

	typedef unsigned int (*iiptr)(unsigned int, unsigned int);
	typedef double (*ddptr)(double, double);
	typedef double (*idptr)(unsigned int, double);
	typedef double (*diptr)(double, unsigned int);

	typedef bool (*biiptr)(unsigned int, unsigned int);
	typedef bool (*bddptr)(double, double);
	typedef bool (*bidptr)(unsigned int, double);
	typedef bool (*bdiptr)(double, unsigned int);

	iiptr fptrs[14][8][8];

	map<unsigned int,map<unsigned int, unsigned int>> methodmap;


public:
	CInterpreter(char*, DebugInfo*);
	virtual ~CInterpreter();
	void start();
	int execute_next();
	void print_a_string(char*);
	void print_a_string(char*, unsigned int);
	void call_external(char*, unsigned int);
	vector<stack_element>* getStack();
	void check_magic_number();
	unsigned int find_offset();
};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
