#include "pProgramNode.h"

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

void pProgramNode::print(int level)

{for (unsigned int i = 0; i < level; i++) {
	 						cout << "+";
	 					}
	 cout << "pProgramNode" << endl; 			}

