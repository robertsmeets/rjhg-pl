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

class CommaSeparated {
public:
  void addIdentifier(string);
};
     class Statement {};

     class Statements { public:
void addStatement(Statement*); };


     class Assignment: public Statement {};
     class Expression {};
     class ExpressionList {
public:
	void addExpression(Expression*);

};
class SingleMethodCall{
public:
   SingleMethodCall(string,ExpressionList*);};
     class CompositeMethodCall : public Statement,public Expression {
public:
   void addSingleMethodCall(SingleMethodCall*);

};
     class ProcedureCall:public Statement,public Expression {};


class Val2Expression:public Expression{
public:
   Val2Expression(char,Expression*, Expression*);
};
class Literal:public Expression{};
class LitInt:public Literal{ public: LitInt(int);};
class LitFloat:public Literal{public: LitFloat(double);};
class LitBool:public Literal{public: LitBool(bool);};
class LitString:public Literal{public: LitString(string);};

#endif 


