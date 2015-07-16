#include "ProcedureCall.h"

ProcedureCall::~ProcedureCall()
{

}

void ProcedureCall::print(int level)

{for (unsigned int i = 0; i < level; i++) {
	 						cout << "+";
	 					}
	 cout << "ProcedureCall" << endl; 			}
