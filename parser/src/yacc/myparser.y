%{
#include <stdio.h>
#include "Assignment.h"
#include "CommaSeparated.h"
#include "CompositeMethodCall.h"
#include "Expression.h"
#include "ExpressionList.h"
#include "LitBool.h"
#include "Literal.h"
#include "LitFloat.h"
#include "LitInt.h"
#include "LitString.h"
#include "pClassDefinition.h"
#include "pMethodDefinition.h"
#include "pProcedureNode.h"
#include "pProgramNode.h"
#include "ProcedureCall.h"
#include "SingleMethodCall.h"
#include "Statement.h"
#include "Statements.h"
#include "Val2Expression.h"
#include "VariableValue.h"

extern char *yytext;
#define YYDEBUG_LEXER_TEXT yytext

%}

// Symbols.
%union
{
    char	*sval;
    pProgramNode *a_program;
    pClassDefinition *a_class;
    pProcedureNode *a_procedure;
    pMethodDefinition *a_method;
    Statements *a_statementlist;
    Statement *a_statement;
    Assignment *an_assignment;
    SingleMethodCall *a_single_methodcall;
    CompositeMethodCall *a_composite_methodcall;
    ProcedureCall *a_procedurecall;
    ExpressionList *an_expressionlist;
    Expression *an_expression;
    CommaSeparated *a_comma_separated;
    CompositeMethodCall *a_restmethodcall;
    Literal *a_literal;
    int Integer;
    double Double;
    bool Boolean;
};
%token <sval> IDENTIFIER
%token PROCEDURE
%token BLOCK
%token ENDBLOCK
%token METHOD
%token CLASS
%token POINT
%token COMMA
%token EQUALS
%token LPAREN
%token RPAREN
%token SEMICOL
%token PLUS
%token MINUS
%token MUL
%token DIV
%token FLOAT
%token STRING
%token BOOLEAN
%token INTEGER

%nonassoc BLOCK
%nonassoc IDENTIFIER

%left PLUS MINUS
%left MUL DIV
%left NEG

%right POINT

%parse-param {pProgramNode *pproot}
%parse-param {char **errmsg}

%type <a_program> Program
%type <a_class> Class
%type <a_procedure> Procedure
%type <a_method> Method
%type <a_program> Highlevelblock
%type <a_statementlist> Statements
%type <a_statement> Statement
%type <an_assignment> Assignment
%type <a_single_methodcall> SingleMethodCall
%type <a_composite_methodcall> CompositeMethodCall
%type <a_procedurecall> ProcedureCall
%type <an_expressionlist> ExpressionList
%type <an_expression> Expression
%type <a_comma_separated> CommaSeparated
%type <a_comma_separated> Instancevariables
%type <a_restmethodcall> RestMethodCall
%type <a_literal> Literal
%type <Integer> INTEGER
%type <Double> FLOAT
%type <Boolean> BOOLEAN

%token <sval> IDENTIFIER
%token PROCEDURE
%start Program


%%

Program:
	/* empty */  {$$ =pproot;}
	|Highlevelblock Program  {$$=pproot;}
	;

Highlevelblock:
	 Class {cout << "add class!" << endl; glob->addClass($1);}
	|Procedure {cout << "add proc!" << endl;glob->addProcedure($1);}
	|Method {cout << "add method!" << endl; glob->addMethodDefinition($1);}
	; {$$=pproot;}

Procedure:
	PROCEDURE IDENTIFIER LPAREN CommaSeparated RPAREN 
	BLOCK 
	Statements	
	ENDBLOCK 
	; {$$=new pProcedureNode($2);}

Class:
	CLASS IDENTIFIER BLOCK 
	Instancevariables
	ENDBLOCK
	; {  $$ = new pClassDefinition($2);}

Method:
	METHOD IDENTIFIER POINT IDENTIFIER LPAREN CommaSeparated RPAREN BLOCK 
	Statements
	ENDBLOCK
	; {  $$ = new pMethodDefinition($2,$4,$9);}

Statements:
	/* empty */ {$$ = new Statements();}
	| Statements Statement {$$ = $1; $1->addStatement($2);}
	;

Statement:
	 Assignment {$$ = $1;}
	|ProcedureCall {$$ = $1;}
	|CompositeMethodCall {$$ = $1;}
	;

Assignment:
	IDENTIFIER EQUALS Expression SEMICOL
	; { $$ = new Assignment();}

ProcedureCall:
	IDENTIFIER LPAREN ExpressionList RPAREN; {$$=new ProcedureCall();}

ExpressionList:
	/* empty */ {$$ = new ExpressionList();}
	| ExpressionList COMMA Expression  { $$=$1; $1->addExpression($3); }
	;

CompositeMethodCall: {$$=new CompositeMethodCall();}
	SingleMethodCall RestMethodCall;

SingleMethodCall:
	IDENTIFIER LPAREN ExpressionList RPAREN; {$$=new SingleMethodCall($1,$3);}

RestMethodCall: 
	/* empty */ {$$ = new CompositeMethodCall();}
	|RestMethodCall POINT SingleMethodCall; {$$=$1;$1->addSingleMethodCall($3);}

Expression:
	 Literal {$$=$1;}
        |IDENTIFIER {$$=new VariableValue($1);}
	|Expression PLUS Expression {$$=new Val2Expression('+',$1,$3);}
	|Expression MINUS Expression {$$=new Val2Expression('-',$1,$3);}
	|Expression MUL Expression {$$=new Val2Expression('*',$1,$3);}
	|Expression DIV Expression {$$=new Val2Expression('/',$1,$3);}
	|LPAREN Expression RPAREN {$$=$2;}
	|CompositeMethodCall {$$=$1;}
	|ProcedureCall {$$=$1;}

Literal:
	 INTEGER {$$=new LitInt($1);}
	|FLOAT {$$=new LitFloat($1);}
	|BOOLEAN {$$=new LitBool($1);}

Instancevariables:
	/* empty */ {$$=new CommaSeparated();}
   | CommaSeparated SEMICOL {$$=$1;}
   ;
   
CommaSeparated:
	IDENTIFIER {$$=new CommaSeparated();$$->addIdentifier($1);}
	|CommaSeparated COMMA IDENTIFIER {$$->addIdentifier($3);}
   
%%
#include <iostream>




using namespace std;
extern "C"
{
        int yylex(void);  
        int yywrap()
        {
                return 1;
        }   
}

int yyerror(pProgramNode*s,char**x,char*y) {
   printf("yyerror : %s\n",y);
}

pProgramNode* glob;

int main(void) {
yydebug = 1;
   glob = new pProgramNode();
   char* errmsg = "BLA";
   int result = yyparse(glob,&errmsg);
   glob->print(0);
}

