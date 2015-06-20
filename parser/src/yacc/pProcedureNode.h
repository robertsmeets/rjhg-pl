#ifndef PPROCEDURENODE_H_
#define PPROCEDURENODE_H_
#include <string>

using namespace std;
class pProcedureNode {
public:
	pProcedureNode(string);
	virtual ~pProcedureNode();
	void print(int);
};

#endif
