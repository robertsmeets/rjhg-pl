/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "CodeGenerator.h"

using namespace std;

CodeGenerator::CodeGenerator() {
	codesize = 100000;
	codebuffer = (char*) malloc(codesize);
	here = 0;
}

CodeGenerator::~CodeGenerator() {
	free(codebuffer);
}

unsigned int CodeGenerator::getHere() {
	return here;
}

char* CodeGenerator::getCodeBuffer() {
	return codebuffer;
}

void CodeGenerator::start(ProgramNode pn) {
	//
	// set up a call to main
	//
	// put a dummy value on the stack
	//
	emit(1, 2, 0);
	//
	// emit INT
	//
	emit(6, 0, 0);
	//
	// emit CAL
	//
	emit(5, 0, 0);
	callpoints[here - 2] = "main";
	//
	// emit RET
	//
	emit(2, 0, 0);
	//
	// generate all the procedures
	//
	vector<ProcedureNode*> procs;
	procs = pn.getProcedures();
	//
	//
	for (vector<ProcedureNode*>::iterator it = procs.begin(); it != procs.end();
			++it) {
		ProcedureNode* a_proc = *it;
		string pname = a_proc->getName();
		a_proc->setProcAddress(here);
		procaddresses[pname] = a_proc;
		start_proc(a_proc);
	}
	//
	// fix the proc addresses
	//
	fix_proc_addresses();
	// printcodebuffer();
}

/**
 * generate the code for a procedure
 */
void CodeGenerator::start_proc(ProcedureNode* a_proc) {
	//
	// emit all the statements
	//
	vector<Statement*>* statements = a_proc->getStatements();
	for (vector<Statement*>::iterator it = statements->begin();
			it != statements->end(); ++it) {
		(*it)->emit(this);
	}
}

//
// emit a f,l,a combination
//
void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a) {
	*((char*) codebuffer + here) = f;
	here++;
	*((char*) codebuffer + here) = l & 255;
	here++;
	*((char*) codebuffer + here) = l >> 8;
	here++;
	*((char*) codebuffer + here) = a & 255;
	here++;
	*((char*) codebuffer + here) = a >> 8;
	here++;
	//printcodebuffer();
}

//
// emit the code for an expression
//
void CodeGenerator::emitRpn(vector<ExpressionThing> vs, ProcedureNode* pn) {
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
		map<string, unsigned int>* local_variables;
		map<string, unsigned int>::iterator foundIter;
		vector<string>* parameters;
		vector<string>::iterator it2;
		double my_double;
		unsigned int strlen;
		unsigned int sz;
		unsigned int bvalue;
		string my_string;
		switch (atype) {
		case 1: // operation
			emitOperation(avalue);
			break;
		case 2: // literal integer
			emit(1, 2, atoi(avalue.c_str()));
			break;
		case 3:  // variable name
			//
			// now we have to look up the variable name.
			// Can be either a local variable or a parameter name.
			//
			local_variables = pn->getLocalVariables();
			foundIter = local_variables->find(avalue);
			if (foundIter == local_variables->end()) {
				parameters = pn->getParameters();
				for (it2 = parameters->begin(); it2 != parameters->end();
						++it2) {
					if ((*it2) == avalue) {
						unsigned int number = it2 - parameters->begin();
						emit(3, 0, local_variables->size() + number); // LOD
						break;
					}
				}
				if (it2 == parameters->end()) {
					throw PException("variable " + avalue + " not found");
				}
			} else {
				emit(3, 0, local_variables->at(avalue)); // LOD
			}
			break;
		case 4: // call
			emit(6, 0, 0);
			emit(5, 0, 0);
			//
			// shorten the proc name (still has "(" at the end)
			//
			addCallAddress(here - 2, avalue.substr(0, avalue.size() - 1));
			break;
		case 5: // float
			my_double = atof(avalue.c_str());
			sz = sizeof(my_double);
			emit(1, 5, sz);
			cout << "--- EMITTING FLOAT " << my_double << " " << sz
					<< " into address <" << codebuffer + here << ">" << endl;
			//cout << "--- The codebuffer itself is at <"<< codebuffer << ">" << endl;
			memcpy(codebuffer + here, &my_double, sz);
			here += sz;
			break;
		case 6: // boolean
			if (avalue == "true") {
				bvalue = 1;
			} else {
				bvalue = 0;
			}
			emit(1, 6, bvalue);
			break;
		case 7: // string
			strlen = avalue.length() - 2;
			my_string = avalue.substr(1, strlen);
			emit(1, 7, strlen);
			memcpy(codebuffer + here, my_string.c_str(), strlen);
			cout << "--- Here is a string [";
			for (unsigned int i = 0; i < strlen; i++) {
				cout << *(codebuffer + here + i);
			}
			cout << "]" << endl;
			here += strlen;
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
	for (map<unsigned int, string>::iterator it = callpoints.begin();
			it != callpoints.end(); ++it) {
		unsigned int call_address = it->first;
		string proc_name = it->second;
		//
		// look up the proc name
		//
		ProcedureNode* pn = procaddresses[proc_name];
		unsigned int proc_address = pn->getProcAddress();
		if (proc_address == 0) {
			cout << "Proc " << proc_name << " not found" << endl;
			throw PException("Proc " + proc_name + " not found");
		}
		//
		// found the address of the proc
		//
		cout << "fixing proc " << proc_name << " call_address " << call_address
				<< " to " << proc_address << endl;
		fix(call_address, proc_address);
		//
		// also fix the INT depth to create room for local variables
		//
		// size of the local variables
		//
		*((char*) codebuffer + call_address - 5) =
				pn->getLocalVariables()->size();
		//
		// size of the parameters
		//
		*((char*) codebuffer + call_address - 7) = pn->getParameters()->size();
		//
		cout << "--- Emitting an INT with local variables size " <<
				 pn->getLocalVariables()->size() << " and parameters size " <<
				 pn->getParameters()->size() << endl;

	}
}

void CodeGenerator::fix(unsigned int call_address, unsigned int dest_address) {
	*((char*) codebuffer + call_address) = dest_address & 255;
	*((char*) codebuffer + call_address + 1) = dest_address >> 8;
}

//
// add a call address (starting point of a proc)
//
void CodeGenerator::addCallAddress(unsigned int address, string proc_name) {
	cout << "adding call: " << address << " calls " << proc_name << endl;
	callpoints[address] = proc_name;
}

void CodeGenerator::printcodebuffer() {
	for (unsigned int i = 0; i < here; i++) {
		cout << "i=" << i << ": " << (unsigned int) (*((char*) codebuffer + i))
				<< endl;
	}
}
