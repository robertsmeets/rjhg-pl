#include "pProcedureNode.h"

#include <iostream>

using namespace std;

 pProcedureNode::pProcedureNode(string name)
{
}

 pProcedureNode::~pProcedureNode()
{
}

 void pProcedureNode::print(int level) {
	 {for (unsigned int i = 0; i < level; i++) {
	 						cout << "+";
	 					}
	 cout << "pProcedureNode" << endl; 			}

 }