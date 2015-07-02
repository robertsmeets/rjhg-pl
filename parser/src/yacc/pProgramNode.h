#ifndef PPROGRAMNODE_H_
#define PPROGRAMNODE_H_

#include "pProcedureNode.h"
#include "pClassDefinition.h"
#include "pMethodDefinition.h"

#include <string>
#include <vector>

using namespace std;
class pProgramNode {
private:
	vector<pProcedureNode*> procedures;
	vector<pClassDefinition*> my_classes;
	vector<pMethodDefinition*> methods;

public:
	pProgramNode();
	virtual ~pProgramNode();
	void addProcedure(pProcedureNode*);
	void addClass(pClassDefinition*);
	void addClassByName(string);
	void addMethodDefinition(pMethodDefinition*);
	void print(int);
};

#endif
