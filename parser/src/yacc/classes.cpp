#include <iostream>
#include <string>

#include "classes.h"

using namespace std;

pProgramNode::pProgramNode()
{
}

pProgramNode::~pProgramNode()
{
}

void pProgramNode::addClass(pClassDefinition* c)
{

cout << "adding class!!!!!!!!!!!!!!!!!" << endl;
}

void pProgramNode::addProcedure(pProcedureNode* c)
{
cout << "adding procedure !!!!!!!!!!!!!!!!" << endl;
}

void pProgramNode::addMethodDefinition(pMethodDefinition* c)
{
cout << "adding method definition!!!!!!!!!!!!!!!!" << endl;
}

 pProcedureNode::pProcedureNode(string name)
{
}

 pProcedureNode::~pProcedureNode()
{
}


pClassDefinition::pClassDefinition(string class_name)
{
cout << "class definition " << class_name << endl;
}

 pClassDefinition::~pClassDefinition()
{
}

pMethodDefinition::pMethodDefinition(string class_nema,string method_name)
{
}


pMethodDefinition::~pMethodDefinition()
{
}

void Statements::addStatement(Statement* s)
{
}


