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
#include "stdio.h"
#include <fstream>

#include "ExpressionParser.h"
#include "pProgramNode.h"
#include "PException.h"
#include "pProcedureNode.h"
#include "ProcedureCallNode.h"
#include "ReturnNode.h"
#include "IfNode.h"
#include "WhileNode.h"
#include "PrintNode.h"
#include "ClassDefinition.h"

using namespace std;

class Parse {
	unsigned int linepos;
	unsigned int charpos;
	unsigned int offset;

	unsigned int last_linepos;
	unsigned int last_charpos;
	unsigned int last_offset;

	unsigned int classnum;
	vector<char, allocator<char> > buffer;
	char found_char;
	string peek_string;
	ExpressionParser ep;
	ProgramNode pn;

public:
	Parse();
	virtual ~Parse();
	ProgramNode* getPn();
	void start(string);
	void parse_from_memory();
	void code_definition();
	void get_something(string);
	bool get_onething(string);
	void class_definition();
	void method_definition();
	void instance_variable_definition();
	void immediate_code();
	void procedure_definition();
	void trim(string&);
	void lookahead();
	vector<Statement*> block(pProcedureNode*);
	Statement* procedure_call(pProcedureNode*);
	Statement* assignment(pProcedureNode*);
	Statement* return_statement(pProcedureNode*);
	Statement* if_statement(pProcedureNode*);
	Statement* while_statement(pProcedureNode*);
	Statement* print_statement(pProcedureNode*);
	vector<char, allocator<char> >* getBuffer();
};

#endif /* PARSE_H_ */
