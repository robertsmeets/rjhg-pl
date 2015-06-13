%{
#include <stdio.h>
#include "classes.h"
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
    MethodCall *a_methodcall;
    ProcedureCall *a_procedurecall;
    ExpressionList *an_expressionlist;
    Expression *an_expression;
    CommaSeparated *a_comma_separated;
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
%type <a_methodcall> MethodCall
%type <a_procedurecall> ProcedureCall
%type <an_expressionlist> ExpressionList
%type <an_expression> Expression
%type <a_comma_separated> CommaSeparated

%token <sval> IDENTIFIER
%token PROCEDURE
%start Program
%%

Program:
	/* empty */  {$$ =pproot;}
	|Highlevelblock Program  {$$=pproot;}
	;

Highlevelblock:
	 Class {$$->addClass($1);}
	|Procedure {$$->addProcedure($1);}
	|Method {$$->addMethodDefinition($1);}
	; {$$=pproot;}

Procedure:
	PROCEDURE IDENTIFIER LPAREN RPAREN {$$=new pProcedureNode($2);}
	BLOCK 
	Statements	
	ENDBLOCK 
	;

Class:
	CLASS IDENTIFIER BLOCK {  $$ = new pClassDefinition($2);}
	Instancevariables
	ENDBLOCK
	;

Method:
	METHOD IDENTIFIER POINT IDENTIFIER BLOCK {  $$ = new pMethodDefinition($2,$4);}
	Statements
	ENDBLOCK
	;

Statements:
	/* empty */
	| Statements Statement {$$ = new Statements(); $$->addStatement($2);}
	;

Statement:
	Assignment;
	ProcedureCall;
	MethodCall;
	;

Assignment:
	IDENTIFIER EQUALS Expression SEMICOL { $$ = new Assignment();}
	;

ProcedureCall:
	IDENTIFIER LPAREN ExpressionList RPAREN;

ExpressionList:
	/* empty */
	| ExpressionList COMMA Expression 
	;

MethodCall:
	IDENTIFIER RestMethodCall;

RestMethodCall: 
	/* empty */
	RestMethodCall POINT IDENTIFIER LPAREN ExpressionList RPAREN;

Expression:
	Literal
	|IDENTIFIER LPAREN Expression RPAREN
	|Expression POINT IDENTIFIER LPAREN Expression RPAREN
	|Expression PLUS Expression
	|Expression MINUS Expression
	|Expression MUL Expression
	|Expression DIV Expression
	|LPAREN Expression RPAREN
	{ printf("EXPRESSION\n");};

Literal:
	INTEGER
	|FLOAT
	|STRING
	|BOOLEAN
	{ printf("LITERAL\n");};

Instancevariables:
	/* empty */
   | CommaSeparated SEMICOL
   ;
   
CommaSeparated:
	IDENTIFIER
	|CommaSeparated COMMA IDENTIFIER
   
%%
#include <iostream>
#include "classes.h"

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
  printf("yyerror : %s\n",s);
}

int main(void) {
  pProgramNode p;
   char* errmsg;
  int result = yyparse(&p,&errmsg);
}

