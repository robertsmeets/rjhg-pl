#ifndef PPROGRAMNODE_H_
#define PPROGRAMNODE_H_

#include "pProcedureNode.h"
#include "pClassDefinition.h"
#include "pMethodDefinition.h"
#include <string>
using namespace std;
class pProgramNode {
public:
	pProgramNode();
	virtual ~pProgramNode();
	void addProcedure(pProcedureNode*);
	void addClass(pClassDefinition*);
	void addClassByName(string);
	void addMethodDefinition(pMethodDefinition*);
};

#endif
