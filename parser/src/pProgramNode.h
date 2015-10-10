/*
 * ProgramNode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */
#ifndef PPROGRAMNODE_H_
#define PPROGRAMNODE_H_

#include "pProcedureNode.h"
#include "pClassDefinition.h"
#include "pMethodDefinition.h"

#include <string>
#include <vector>

using namespace std;

class pProcedureNode; // forward declaration

class pProgramNode {
private:
	vector<pProcedureNode*> procedures;
	vector<pClassDefinition*> my_classes;
	vector<pMethodDefinition*> methods;
	map<string, unsigned int> method_numbers;
	unsigned int next_methodnumber;

public:
	pProgramNode();
	virtual ~pProgramNode();
	pClassDefinition* getClass(string);
	unsigned int assignMethodNumber(string);
	unsigned int getMethodNumber(string);
	void addProcedure(pProcedureNode*);
	void addClass(pClassDefinition*);
	void addClassByName(string);
	void addMethodDefinition(pMethodDefinition*);
	void print(int);
   vector<pClassDefinition*> getClasses();
   vector<pProcedureNode*> getProcedures();

};

#endif
