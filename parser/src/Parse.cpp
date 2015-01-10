/*
 * Parse.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: robert
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "Parse.h"
#include "PException.h"
#include "ExpressionParser.h"
#include "ProcedureNode.h"
#include "ProcedureCallNode.h"

using namespace std;

Parse::Parse() {
	found_char = ' ';
	offset = 0;
	ep = ExpressionParser();
}

Parse::~Parse() {

}

ProgramNode Parse::getPn() {
	return pn;
}

void Parse::start() {
	string filename = "F:\\robert\\projects\\parser\\code\\example.src";
	ifstream myfile(filename.c_str(), ios::binary);
	streampos begin, end;
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	int size = end - begin;
	//
	// reading it into a buffer
	//
	vector<char> a_buffer(size);
	buffer = a_buffer;
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (!myfile.read(buffer.data(), size)) {
		cout << "File " << filename << " could not be opened" << endl;
	}
	//
	// buffer now contains it
	//
	myfile.close();
	try {
		parse_from_memory();
	} catch (PException & E) {
		cout << "Caught exception type: " << E.ShowReason() << endl;
	}

}

void Parse::parse_from_memory() {
	//
	// read the contents from buffer, and make a parse tree
	//
	offset = 0;
	get_something(" \n\t\r");
	for (;;) {
		code_definition();
		if (peek_string.empty()) {
			return;

		}
	}
}

bool Parse::get_something(string chars) {
	for (;;) {
		bool result = get_onething(chars);
		if (!result) {
			return false;
		}
		if (!peek_string.empty()) {
			break;
		}
	}
	return true;
}

void Parse::code_definition() {
	cout << "in code_definition peek_string = <" << peek_string << ">" << endl;
	if (peek_string == "class") {
		class_definition();
	} else if (peek_string == "method") {
		method_definition();
	} else if (peek_string == "instance_variable") {
		instance_variable_definition();
	} else if (peek_string == "proc") {
		procedure_definition();
	} else {
		immediate_code();
	}
}

void Parse::trim(string& s) {
	s.erase(s.find_last_not_of(" \n\r\t") + 1);
	unsigned int first_useful = s.find_first_not_of(" \n\r\t");
	if ((first_useful > 0) && (first_useful != string::npos)) {
		s.erase(0, first_useful);
	}
}

bool Parse::get_onething(string chars) {
	if (chars == "") {
		chars = " \n\t\r";
	}
	peek_string = "";
	found_char = ' ';
	for (;;) {
		if (offset >= buffer.size()) {
			return false;
		}
		char c = buffer[offset];
		offset++;
		unsigned int found = chars.find(c);
		// -1: not found
		if (found == string::npos) {
			// strip the peek_string?
			peek_string += c;
		} else {
			found_char = c;
			trim(peek_string);
			break;
		}
	}
	return true;
}

void Parse::class_definition() {
	cout << "class_definition" << endl;
	get_something(" \n\t\r");
	//
	// get the class name
	//
	string class_name = peek_string;
	get_something(" \n\t\r");
	//
	// now expect instance_variables, or method keyword
	//
	if (peek_string == "instance_variables") {
		instance_variable_definition();
	} else if (peek_string == "method") {
		method_definition();
	} else {
		cout << "expected instance_variables or method but got <" << peek_string
				<< ">" << endl;
	}

}
void Parse::method_definition() {

	cout << "method_definition" << endl;
	get_something(" \n\t\r");
	string method_name = peek_string;
	get_something(" \n\t\r");

}

void Parse::procedure_definition() {
	ProcedureNode* pd = new ProcedureNode();
	//
	// get the definition
	//
	get_something("(");
	string proc_name = peek_string;
	cout << "PROCEDURE_DEFINITION " << proc_name << endl;
	pd->setName(proc_name);
	for (;;) {
		get_something("),");
		pd->addParameter(peek_string);
		if (found_char == ')') {
			// done
			break;
		}
	}
	//
	// get the instance variables
	//
	vector<string> instance_variables;
	for (;;) {
		get_something(" \n\t\r");
		if (peek_string == "end") {
			get_something(" \n\t\r");
			break;
		}
		pd->addInstanceVariable(peek_string);
	}
	//
	// get the procedure body
	//
	if (peek_string != "begin") {
		throw PException("expected begin, received " + peek_string);
	}
	//
	// get an assignment, procedure call, or end
	//
	for (;;) {
		get_something("(=\n\r");
		cout << "peek_string = " << peek_string << endl;
		cout << "found_char = " << found_char << endl;
		if (peek_string == "end") {
			get_something(" \n\t\r");
			break;
		}
		//
		// assignment or proc call
		//
		if (found_char != '=') {
			//
			// assume procedure call
			//
			procedure_call(pd);
		} else {
			//
			// must be an assignment
			//
			string assignment_left = peek_string;
			get_something("\n\t\r");
			//
						// create assignment node with new, to avoid it going out of scope
						//
						AssignmentNode* an = new AssignmentNode();
			//
			// look up the instance variable
			//
			unsigned int i = pd->getInstanceVariable(assignment_left);
			an->setLhs(i);
			string assignment_right = peek_string;
			ExpressionNode* en = ep.parse(assignment_right);
			an->setRhs(en);
			pd->addStatement((Statement*)an);
		}
	}
	pn.addProcedure(pd);
}

void Parse::instance_variable_definition() {

	cout << "instance_variable_definition" << endl;
	for (;;) {
		get_something(" \n\t\r");
		if (peek_string == "end") {
			get_something(" \n\t\r");
			return;
		}
		cout << "INSTANCE VARIABLE " << peek_string << endl;
	}

}

void Parse::local_variable_definition() {
}

void Parse::immediate_code() {

	throw PException("unexpected string " + peek_string);

}

void Parse::procedure_call(ProcedureNode* pd) {
	cout << "---------------------> procedure_call" << endl;
	string proc_name = peek_string;
	cout << "name " << proc_name << endl;
	for (;;) {
		get_something("),");
		cout << "parameter " << peek_string << endl;
		;
		if (found_char == ')') {
			// done
			break;
		}
	}
	ProcedureCallNode* pcn = new ProcedureCallNode();
	pcn->setProcedureName(proc_name);
	pd->addStatement((Statement*) pcn);
}
