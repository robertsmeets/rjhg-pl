/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */


#include "CodeGenerator.h"

using namespace std;

CodeGenerator::CodeGenerator() {
	codebuffer = vector<unsigned char>();
	procadresses = map<string, unsigned int>();
	callpoints = map<unsigned int, string>();
	here = 0;
}

CodeGenerator::~CodeGenerator() {

}

unsigned int CodeGenerator::getHere() {
	return here;
}

void CodeGenerator::start(ProgramNode pn) {
	//
	// open a file
	//
	string filename = "F:\\robert\\projects\\parser\\code\\example.bin";
	vector<ProcedureNode*> procs;
	procs = pn.getProcedures();
	//
	//
	for (vector<ProcedureNode*>::iterator it = procs.begin(); it != procs.end();
			++it) {
		ProcedureNode* a_proc = *it;
		string pname = a_proc->getName();
		procadresses[pname] = here;
		start_proc(a_proc);
	}
	//
	// fix the proc addresses
	//
	fix_proc_addresses();
	myfile.open(filename.c_str(), ios::binary);
	emit_to_file();
	myfile.close();
}

/**
 * generate the code for a procedure
 */
void CodeGenerator::start_proc(ProcedureNode* a_proc) {
	//
	// emit all the statements
	//
	vector<Statement*>* statements = a_proc->getStatements();
	for (vector<Statement*>::iterator it = statements->begin();it != statements->end(); ++it)
	{
		(*it)->emit(this);
	}
}

//
// write the codebuffer to a file
//
void CodeGenerator::emit_to_file() {
	for (unsigned int i = 0; i < here; i++) {
		char c = codebuffer[i];
		myfile.write(reinterpret_cast<const char*>(&c), sizeof(c));
	}
}

//
// print the codebuffer
//
void CodeGenerator::printcodebuffer() {
	cout << "-------here is " << here << endl;
	for (unsigned int i = 0; i < here; i++) {
		cout << "codebuffer[" << i << "] = " << (unsigned int) (codebuffer[i])
				<< endl;
		;
	}
	cout << "--- end" << endl;

}

//
// emit a f,l,a combination
//
void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a) {
	// cout << "emit " << (unsigned int) f << "," << l << "," << a << endl;
	codebuffer.push_back(f);
	here++;
	codebuffer.push_back(l);
	here++;
	codebuffer.push_back(l >> 8);
	here++;
	codebuffer.push_back(a);
	here++;
	codebuffer.push_back(a >> 8);
	here++;
	// printcodebuffer();

}

//
// emit the code for an expression
//
void CodeGenerator::emitRpn(vector<ExpressionThing> vs) {
	for (vector<ExpressionThing>::iterator it = vs.begin(); it != vs.end();
			++it) {
		//
		// type 1: operation
		// type 2: literal integer
		// type 3: variable name
		// type 4: proc call
		//
		int atype = (*it).getType();
		string avalue = (*it).getValue();
		switch (atype) {
		case 1: // operation
			emitOperation(avalue);
			break;
		case 2: // literal integer
			emit(1, 0, atoi(avalue.c_str()));
			break;
		case 3:  // variable name
			emit(3, 0, 0); // LOD
			break;
		case 4: // call
			//
			// shorten the proc name (still has "(" at the end)
			//
			emit(5, 0, 0);
			addCallAddress(here - 2, avalue.substr(0,avalue.size() -1));
			break;
		default:
			throw PException("Unexpected ExpressionThing type");
			break;
		}
	}
}

//
// emit the code for an operation
//
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

/**
 * correct the procedure call addresses
 *
 */
void CodeGenerator::fix_proc_addresses() {
	//
	// loop over the callpoints
	//
	cout << "fix_proc_addresses" << endl;
	for (map<unsigned int, string>::iterator it = callpoints.begin();
			it != callpoints.end(); ++it) {
		unsigned int call_address = it->first;
		string proc_name = it->second;
		//
		// look up the proc name
		//
		unsigned int proc_address = procadresses[proc_name];
		if (proc_address == 0) {
			cout << "Proc " << proc_name << " not found" << endl;
			throw PException("Proc " + proc_name + " not found");
		}
		//
		// found the address of the proc
		//
		cout << "fixing proc " << proc_name << " call_address " << call_address
				<< " to " << proc_address << endl;
		fix(call_address,proc_address);
	}
}

void CodeGenerator::fix(unsigned int call_address,unsigned int dest_address)
{
	codebuffer[call_address] = dest_address & 255;
	codebuffer[call_address + 1] = dest_address >> 8;
}

//
// add a call adress (starting point of a proc)
//
void CodeGenerator::addCallAddress(unsigned int address, string proc_name) {
	cout << "adding call: " << address << " calls " << proc_name << endl;
	callpoints[address] = proc_name;
}

