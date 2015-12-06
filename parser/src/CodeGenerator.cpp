/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "CodeGenerator.h"

using namespace std;

CodeGenerator::CodeGenerator() {
	codesize = 60000;
	codebuffer = (char*) malloc(codesize);
	here = 0;
	pn = NULL;
	di = NULL;
	opr_mapping["+"] = 2;
	opr_mapping["-"] = 3;
	opr_mapping["*"] = 4;
	opr_mapping["/"] = 5;
	opr_mapping["%"] = 6;
	opr_mapping["=="] = 7;
	opr_mapping["!="] = 8;
	opr_mapping["<"] = 10;
	opr_mapping[">="] = 11;
	opr_mapping[">"] = 12;
	opr_mapping["<="] = 13;
}

CodeGenerator::~CodeGenerator() {
	free(codebuffer);
}

uint16_t CodeGenerator::getHere() {
	return here;
}

char* CodeGenerator::getCodeBuffer() {
	return codebuffer;
}

void CodeGenerator::start(pProgramNode* a_pn, DebugInfo* a_di) {
	cout << "Code generation..." << endl;
	//
	// emit a magic number
	//
	emitByte('R');
	emitByte('J');
	emitByte('H');
	emitByte('G');
	emitByte('P');
	emitByte('L');
	//
	// leave some space for the start address of the code
	//
	emit2Byte(0);
	//
	// count the number of methods
	//
	uint16_t amount_of_methods = 0;
	for (auto const &a_class : a_pn->getClasses()) {
		amount_of_methods += a_class->getMethods().size();
	}
	//
	// save the start address of the code
	//
	uint16_t offset = 8 * amount_of_methods + 8;
	*((char*) codebuffer + 6) = offset & 255;
	*((char*) codebuffer + 7) = offset >> 8;
	here = offset;
	//
	//
	di = a_di;
	pn = a_pn;
	//
	// set up a call to main
	//
	// put a dummy value on the stack
	//
	emit(1, 2, 0, NULL);
	//
	// emit INT
	//
	emit(6, 0, 0, NULL);
	//
	// emit CAL
	//
	emit(5, 0, 0, NULL);
	callpoints[here - 2] = "main";
	//
	// emit RET
	//
	emit(2, 0, 0, NULL);
	//
	// generate all the procedures
	//
	vector<pProcedureNode*> procs;
	procs = pn->getProcedures();
	//
	// generate the code for all the procedures
	//
	for (vector<pProcedureNode*>::iterator it = procs.begin(); it != procs.end();
			++it) {
		pProcedureNode* a_proc = *it;
		string pname = a_proc->getName();
		a_proc->setProcAddress(here);
		procaddresses[pname] = a_proc;
		start_proc(a_proc);
	}
	//
	// also the methods
	//
	for (auto const &a_class : a_pn->getClasses()) {
		for (auto const &a_method : a_class->getMethods()) {
			a_method->setProcAddress(here);
			start_method(a_method);
		}
	}
	//
	// fix the proc addresses
	//
	fix_proc_addresses();
	//
	// emit the method table
	//
	uint16_t the_index = 8;
	for (auto const &a_class : a_pn->getClasses()) {
		for (auto const &a_method : a_class->getMethods()) {
			uint16_t cnum = a_class->getClassNum();
			uint16_t mnum = a_method->getMethodNumber();
			uint16_t address = a_method->getProcAddress();
			*((char*) codebuffer + the_index) = cnum & 255;
			the_index++;
			*((char*) codebuffer + the_index) = cnum >> 8;
			the_index++;
			*((char*) codebuffer + the_index) = mnum & 255;
			the_index++;
			*((char*) codebuffer + the_index) = mnum >> 8;
			the_index++;
			*((char*) codebuffer + the_index) = address & 255;
			the_index++;
			*((char*) codebuffer + the_index) = address >> 8;
			the_index++;
			*((char*) codebuffer + the_index) =
					a_method->getParameters()->size();
			the_index++;
			*((char*) codebuffer + the_index) =
					a_method->getLocalVariables()->size();
			the_index++;
		}
	}
	cout << "Generated " << here << " bytes of code " << endl;
}

/**
 * generate the code for a procedure
 */
void CodeGenerator::start_proc(pProcedureNode* a_proc) {
	//
	// emit all the statements for a procedure
	//
	Statements* statements = a_proc->getStatements();
	vector<Expression*> expressions =statements->getStatements();
	for (auto const &it :expressions) {
		it->emit(this);
	}
}

//
// emit a f,l,a combination
//
void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a,
		Expression* s) {
	if (s != NULL) {
		 // di->setPosition(here, s->getLinepos(), s->getCharpos(), s->getAbspos());
	}
	emitByte(f);
	emit2Byte(l);
	emit2Byte(a);
}

void CodeGenerator::emitByte(char b) {
	*((char*) codebuffer + here) = b;
	here++;
}

void CodeGenerator::emit2Byte(uint16_t val) {
	emitByte(val & 255);
	emitByte(val >> 8);
}

//
// emit the code for an expression
//
void CodeGenerator::emitRpn(vector<ExpressionThing> vs, pProcedureNode* pn,
		Expression* s) {
	/* for (vector<ExpressionThing>::iterator it = vs.begin(); it != vs.end();
			++it) {
		//
		// type 1: operation
		// type 2: literal integer
		// type 3: variable name
		// type 4: proc call
		//
		int atype = (*it).getType();
		string avalue = (*it).getValue();
		map<string, uint16_t>* local_variables;
		map<string, uint16_t>::iterator foundIter;
		vector<string>* parameters;
		vector<string>::iterator it2;
		double my_double;
		uint16_t strlen;
		uint16_t sz;
		uint16_t bvalue;
		string my_string;
		switch (atype) {
		case 1: // operation
			emitOperation(avalue, s);
			break;
		case 2: // literal integer
			emit(1, 2, atoi(avalue.c_str()), s);
			break;
		case 3:  // variable name
			//
			// now we have to look up the variable name.
			// Can be either a local variable, a parameter name or an instance variable
			//
			local_variables = pn->getLocalVariables();
			foundIter = local_variables->find(avalue);
			if (foundIter == local_variables->end()) {
				parameters = pn->getParameters();
				for (it2 = parameters->begin(); it2 != parameters->end();
						++it2) {
					if ((*it2) == avalue) {
						uint16_t number = it2 - parameters->begin();
						//
						// it is a parameter
						//
						emit(3, 0, number, s); // LOD
						break;
					}
				}
				if (it2 == parameters->end()) {
					//
					// look for instance variable
					//
					uint16_t j = pn->getInstanceVarNum(avalue);
					emit(13, j, 0, s); // LODI
				}
			} else {
				//
				// it is a local variable
				//
				emit(3, 0,
						pn->getParameters()->size()
								+ local_variables->at(avalue), s); // LOD
			}
			break;
		case 4: // call
			//
			// shorten the proc name (still has "(" at the end)
			//
			cout << "emitrpn " << avalue << endl;
			addCallToProc(avalue.substr(0, avalue.size() - 1), s);
			break;
		case 5: // float
			my_double = atof(avalue.c_str());
			sz = sizeof(my_double);
			emit(1, 5, sz, s);
			memcpy(codebuffer + here, &my_double, sz);
			here += sz;
			break;
		case 6: // boolean
			if (avalue == "true") {
				bvalue = 1;
			} else {
				bvalue = 0;
			}
			emit(1, 6, bvalue, s);
			break;
		case 7: // string
			strlen = avalue.length() - 2;
			my_string = avalue.substr(1, strlen);
			emit(1, 7, strlen, s);
			memcpy(codebuffer + here, my_string.c_str(), strlen);
			here += strlen;
			break;
		default:
			throw PException("Unexpected ExpressionThing type");
			break;
		}
	}  */
}

//
// emit the code for an operation
//
void CodeGenerator::emitOperation(string avalue, Expression* s) {
	uint16_t atype = opr_mapping[avalue];
	if (atype == 0) {
		throw PException("Unexpected Operation" + avalue);
	} else {
		emit(2, 0, atype, s);
	}
}

/**
 * correct the procedure call addresses
 *
 */
void CodeGenerator::fix_proc_addresses() {
	//
	// loop over the callpoints
	//
	for (map<uint16_t, string>::iterator it = callpoints.begin();
			it != callpoints.end(); ++it) {
		uint16_t call_address = it->first;
		string proc_name = it->second;
		//
		// look up the proc name
		//
		pProcedureNode* pn = procaddresses[proc_name];
		if (pn == NULL) {
			//
			// external function
			//
			// fix the INT depth to create room for local variables
			//
			// size of the local variables (zero)
			//
			*((char*) codebuffer + call_address - 5) = 0;
			//
			// size of the parameters (assume 1)
			//
			*((char*) codebuffer + call_address - 7) = 1;

		} else {
			uint16_t proc_address = pn->getProcAddress();
			//
			// found the address of the proc
			//
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
			*((char*) codebuffer + call_address - 7) =
					pn->getParameters()->size();
		}

	}
}

void CodeGenerator::fix(uint16_t call_address, uint16_t dest_address) {
	*((char*) codebuffer + call_address) = dest_address & 255;
	*((char*) codebuffer + call_address + 1) = dest_address >> 8;
}

//
// add a call address (starting point of a proc)
//
void CodeGenerator::addCallAddress(uint16_t address, string proc_name) {
	callpoints[address] = proc_name;
}

void CodeGenerator::addCallToProc(string name, Expression* s) {
	//
	//
	cout << "CodeGenerator::addCallToProc(string name, Expression* s) {" << name << endl;
	pClassDefinition* a_class = pn->getClass(name);
	if (a_class != NULL) {
		//
		// it's a class constructor
		//
		addCallToClassConstructor(a_class, s);
	} else {

		if (name.find('.') != string::npos)
		{
			//
			// it's a method call
			//



		//			emitRpn(LhsExpression.getRpn(), pn, this);

//			addCallToMethod(procedure_name, this);

cout << "METHOD CALL " << endl;







		}
		else
		{
		//
		// it's a procedure
		//
		addCallToProcedure(name, s);
		}
	}















}

void CodeGenerator::addCallToMethod(string method_name, Expression* s) {
	//
	//
	uint16_t method_number = pn->getMethodNumber(method_name);
	emit(12, method_number, 0, s);
}

void CodeGenerator::addCallToClassConstructor(pClassDefinition* cd,
		Expression* s) {
	uint16_t ivs = cd->getInstanceVariables().size();
	uint16_t classnum = cd->getClassNum();
	emit(11, classnum, ivs, s);
}

void CodeGenerator::addCallToProcedure(string procedure_name, Expression* s) {
cout << "CodeGenerator::addCallToProcedure(string procedure_name, Expression* s) " << procedure_name << endl;
//
// add room for the local variables.
// emit an INT
// Since we don't know how many, leave 0 for the INT parameter
// this will be corrected in the fix stage
//
	emit(6, 0, 0, s);
//
// determine if procedure_name was defined
// in the program code, if not it's a dynamic call
//
	Expression* proc = procDefined(procedure_name);
	if (proc != NULL) {
		//
		// emit a "cal"
		// leave the call address 0, since this will be corrected in the fix stage
		//
		emit(5, 0, 0, s);
		addCallAddress(here - 2, procedure_name);
	} else {
		//
		// dealing with a dynamic call, to a library function
		// The string is saved.
		//
		throw PException("Procedure not found <" +procedure_name + ">");
		uint16_t strlen = procedure_name.length();
		emit(10, 1, strlen, s);
		addCallAddress(here - 2, procedure_name);
		memcpy(codebuffer + here, procedure_name.c_str(), strlen);
		here += strlen;
	}
}

Expression* CodeGenerator::procDefined(string procedure_name) {
	vector<pProcedureNode*> procedures = pn->getProcedures();
	for (vector<pProcedureNode*>::iterator it = procedures.begin();
			it != procedures.end(); ++it) {
		pProcedureNode* a_proc = *it;
		string pname = a_proc->getName();
		if (pname == procedure_name) {
			return (Expression*) a_proc;
		}
	}
	return NULL;
}


void CodeGenerator::start_method(pMethodDefinition* md)
{
}

char* CodeGenerator::allot(int size)
{
	uint16_t loc = here;
	here += size;
	return codebuffer + loc;
}

