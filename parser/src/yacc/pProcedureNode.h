#ifndef PPROCEDURENODE_H_
#define PPROCEDURENODE_H_
#include <string>

#include "Statements.h"

using namespace std;
class pProcedureNode {
	Statements* statements;
public:
	pProcedureNode(string, Statements*);
	virtual ~pProcedureNode();
	void print(int);
};

#endif
