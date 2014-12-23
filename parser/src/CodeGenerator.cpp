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
		unsigned int lhs = an_assignment.getLhs();
		ExpressionNode rhs = an_assignment.getRhs();
		//
		// emit the calculation instructions
		//
		emitRpn(rhs.getRpn());
		//
		// emit a "sto" to store the value in a variable
		//
		emit(4, 0, lhs);
	}

}

void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a) {
	cout << "emitting " << static_cast<unsigned>(f) << "," << l << "," << a
			<< endl;
	myfile.write(reinterpret_cast<const char*>(&f), sizeof(f));
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
			emit(1, 0,atoi(avalue.c_str()));
			break;
		case 3:  // variable name
			cout << "variable" << endl;
			emit(3, 0, 0); // LOD
			break;
		default:
			throw PException("Unexpected ExpressionThing type");
			break;
		}
	}
}

void CodeGenerator::emitOperation(string avalue) {
	if (avalue == "+")
		emit(2, 0, 2);
	else if (avalue == "-")
		emit(2, 0, 3);
	else if (avalue == "*")
		emit(2, 0, 4);
	else if (avalue == "/")
		emit(2, 0, 5);
	else if (avalue == "%")
		emit(2, 0, 6);
	else if (avalue == "==")
		emit(2, 0, 7);
	else if (avalue == "!=")
		emit(2, 0, 8);
	else if (avalue == "<")
		emit(2, 0, 10);
	else if (avalue == ">=")
		emit(2, 0, 11);
	else if (avalue == ">")
		emit(2, 0, 12);
	else if (avalue == "<=")
		emit(2, 0, 13);
	else
		throw PException("Unexpected Operation" + avalue);

}
