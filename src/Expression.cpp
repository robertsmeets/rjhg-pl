#include "Expression.h"

#include <iostream>

using namespace std;

void Expression::emit(CodeGenerator*, ProcedureNode* pn) {
	//
	// type 1: operation
	// type 2: literal integer
	// type 3: variable name
	// type 4: proc call
	//
/*	int atype = (*it).getType();
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
			for (it2 = parameters->begin(); it2 != parameters->end(); ++it2) {
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
					pn->getParameters()->size() + local_variables->at(avalue),
					s); // LOD
		}
		break;
	case 4: // call
		//
		// shorten the proc name (still has "(" at the end)
		//
		printf("emitrpn " << avalue );
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
		puts("Unexpected ExpressionThing type");
		break;
	}
*/
}

