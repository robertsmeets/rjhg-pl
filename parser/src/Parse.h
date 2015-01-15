/*
 * Parse.h
 *
 *  Created on: Dec 15, 2014
 *      Author: robert
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <vector>
#include <string>
#include <map>
#include "ExpressionParser.h"
#include "ProgramNode.h"

using namespace std;

class Parse {
	vector<char, allocator<char> > buffer;
	char found_char;
	string peek_string;
	unsigned int offset;
	ExpressionParser ep;
	ProgramNode pn;

public:
	Parse();
	virtual ~Parse();
	ProgramNode getPn();
	void start(string);
	void parse_from_memory();
	void code_definition();
	void get_something(string);
	bool get_onething(string);
	void class_definition();
	void method_definition();
	void instance_variable_definition();
	void local_variable_definition();
	void immediate_code();
	void procedure_definition();
	void trim(string&);
	void procedure_call(ProcedureNode*);
	void return_statement(ProcedureNode*);
	void lookahead();
};

#endif /* PARSE_H_ */
