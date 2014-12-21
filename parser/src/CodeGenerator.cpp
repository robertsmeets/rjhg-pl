/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include <iostream>
#include <string>
#include <cstdlib>

#include "CodeGenerator.h"
#include "ProgramNode.h"
#include "AssignmentNode.h"
#include "ExpressionThing.h"
#include "PException.h"

using namespace std;

CodeGenerator::CodeGenerator() {

}

CodeGenerator::~CodeGenerator() {

}

void CodeGenerator::start(ProgramNode pn) {
	cout << "Generating code..." << endl;
	//
	// open a file
	//
	string filename = "F:\\robert\\projects\\parser\\code\\example.bin";
	myfile.open(filename.c_str(), ios::binary);
	vector<ProcedureNode> procs;
	procs = pn.getProcedures();
	//
	// only generate code for the main procedure
	//
	cout << "there are " << procs.size() << " procs" << endl;
	for (vector<ProcedureNode>::iterator it = procs.begin(); it != procs.end();
			++it) {
		ProcedureNode a_proc = *it;
		string pname = a_proc.getName();
		if (pname == "main") {
			cout << "found main" << endl;
			start_proc(a_proc);
		} else {
			cout << "found something else: <" << pname << ">" << endl;

		}
	}
	myfile.close();
}

void CodeGenerator::start_proc(ProcedureNode a_proc) {
	vector<AssignmentNode> assignments = a_proc.getAssignments();
	for (vector<AssignmentNode>::iterator it = assignments.begin();
			it != assignments.end(); ++it) {
		cout << "assignment" << endl;
		AssignmentNode an_assignment = *it;
		string lhs = an_assignment.getLhs();
		ExpressionNode rhs = an_assignment.getRhs();
		//
		// emit the calculation instructions
		//
		emitRpn(rhs.getRpn());
		//
		// emit a "sto" to store the value in a variable
		//
		emit(4, 0, 0);
	}

}

void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a) {
	cout << "emitting " << f << "," << l << ","<< a << endl;
	myfile << f;
	myfile.write(reinterpret_cast<const char*>(&l), sizeof(l));
	myfile.write(reinterpret_cast<const char*>(&a), sizeof(a));

}

void CodeGenerator::emitRpn(vector<ExpressionThing> vs) {
	for (vector<ExpressionThing>::iterator it = vs.begin(); it != vs.end();
			++it) {
		//
		// type 1: operation
		// type 2: literal integer
		// type 3: variable name
		//
		int atype = (*it).getType();
		string avalue = (*it).getValue();
		switch (atype) {
		case 1: // operation
			emitOperation(avalue);
			break;
		case 2: // literal integer
			emit(1, atoi(avalue.c_str()), 0);
			break;
		case 3:  // variable name
			cout << "variable" << endl;
			break;
		default:
			throw PException("Unexpected ExpressionThing type");
			break;
		}
	}
}

void CodeGenerator::emitOperation(string avalue) {
	if(avalue == "+") emit(2, 2, 0);
else if (avalue == "-") emit(2, 3, 0);
else if (avalue == "*") emit(2, 4, 0);
else if (avalue == "/") emit(2, 5, 0);
else if (avalue == "%") emit(2, 6, 0);
else if (avalue == "==") emit(2, 7, 0);
else if (avalue =="!=") emit(2, 8, 0);
else if (avalue =="<") emit(2, 10, 0);
		else if (avalue ==">=") emit(2, 11, 0);
		else if (avalue ==">") emit(2, 12, 0);
		else if (avalue =="<=") emit(2, 13, 0);
		else
		throw PException("Unexpected Operation" + avalue);

	}
