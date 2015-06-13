#ifndef CLASSES_H_
#define CLASSES_H_ 

#include <string>

using namespace std;

class pClassDefinition;

class pProcedureNode;

class pMethodDefinition;

class pProgramNode {
public:
	pProgramNode();
	virtual ~pProgramNode();
	void addProcedure(pProcedureNode*);
	void addClass(pClassDefinition*);
	void addClassByName(string);
	void addMethodDefinition(pMethodDefinition*);
};

class pProcedureNode {
public:
	pProcedureNode(string);
	virtual ~pProcedureNode();
};


class pClassDefinition {
public:
	pClassDefinition(string);
	virtual ~pClassDefinition();
};

class pMethodDefinition {
public:
	pMethodDefinition(string,string);
	virtual ~pMethodDefinition();
};

class CommaSeparated {};
     class Statement {};

     class Statements { public:
void addStatement(Statement*); };


     class Assignment {};
     class MethodCall {};
     class ProcedureCall {};
     class ExpressionList {};
     class Expression {};


#endif 

