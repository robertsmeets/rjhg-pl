%{
#include <stdio.h>
#include "Assignment.h"
#include "If.h"
#include "Return.h"
#include "While.h"
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
#include "Val2Expression.h"
#include "VariableValue.h"

extern char *yytext;
#define YYDEBUG_LEXER_TEXT yytext

%}

%union
{
    char* sval;
    pProgramNode *a_program;
    pClassDefinition *a_class;
    pProcedureNode *a_procedure;
    pMethodDefinition *a_method;
    Assignment *an_assignment;
    If *an_if;
    Return* a_return;
    While* a_while;
    SingleMethodCall *a_single_methodcall;
    CompositeMethodCall *a_composite_methodcall;
    ProcedureCall *a_procedurecall;
    ExpressionList *an_expressionlist;
    Expression *an_expression;
    Statements *a_statements;
    CommaSeparated *a_comma_separated;
    CompositeMethodCall *a_restmethodcall;
    Literal *a_literal;
    VariableValue *a_variablevalue;
    int Integer;
    double Double;
    bool Boolean;
};

%token IDENTIFIER
%token PROCEDURE
%token BLOCK
%token ENDBLOCK
%token METHOD
%token CLASS
%token POINT
%token COMMA
%token EQUALS
%token SEQUALS
%token GT
%token LT
%token GE
%token LE
%token NE
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
%token RETURN
%token WHILE
%token IF
%token ELSE

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
%type <an_assignment> Assignment
%type <an_if> If 
%type <a_return> Return
%type <a_while> While
%type <a_single_methodcall> SingleMethodCall
%type <a_composite_methodcall> CompositeMethodCall
%type <a_procedurecall> ProcedureCall
%type <an_expressionlist> ExpressionList
%type <an_expression> Expression
%type <a_statements> BSB
%type <a_statements> Statements
%type <a_comma_separated> CommaSeparated
%type <a_restmethodcall> RestMethodCall
%type <a_literal> Literal
%type <a_variablevalue> Lhs
%type <Integer> INTEGER
%type <Double> FLOAT
%type <Boolean> BOOLEAN
%type <sval> IDENTIFIER

%token PROCEDURE
%start Program


%%

Program:
	/* empty */  {$$ =pproot;}
	|Program Highlevelblock {$$=pproot;}
	;

Highlevelblock:
	 Class { glob->addClass($1);}
	|Procedure { glob->addProcedure($1);}
	|Method { glob->addMethodDefinition($1);}
	; {$$=pproot;}

Procedure:
	PROCEDURE IDENTIFIER LPAREN CommaSeparated RPAREN 
	BSB ; {$$=new pProcedureNode($2,$6);}

Class:
	CLASS IDENTIFIER BLOCK CommaSeparated SEMICOL ENDBLOCK
	; {  $$ = new pClassDefinition($2);}

Method:
	METHOD IDENTIFIER POINT IDENTIFIER LPAREN CommaSeparated RPAREN
	BSB
	; {  $$ = new pMethodDefinition($2,$4,$8);}

Statements:
	/* empty */ { $$ = new Statements(); }
	| Statements Expression { $$ = $1; $1->addStatement($2);}
	; 

BSB:
	BLOCK Statements ENDBLOCK {$$ = $2;}
	;

Assignment:
	Lhs EQUALS Expression SEMICOL
	; { $$ = new Assignment($1,$3);}

Return:
	RETURN Expression SEMICOL 
        ; { $$ = new Return($2); }

While:
	WHILE Expression BSB
        ; { $$ = new While($2,$3); }

If:
	IF Expression BSB { $$ = new If($2,$3,NULL); }
       |IF Expression BSB ELSE BSB { $$ = new If($2,$3,$5); }
       ;

Lhs:
	IDENTIFIER ; { $$ = new VariableValue($1);}

ProcedureCall:
	IDENTIFIER LPAREN ExpressionList RPAREN ; { $$=new ProcedureCall($1,$3); }

ExpressionList:
	 /* empty */ {$$ = new ExpressionList();}
        | Expression { $$ = new ExpressionList();$$->addExpression($1);}
	| ExpressionList COMMA Expression  { $$ = $1; $1->addExpression($3); }
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
        |IDENTIFIER { $$=new VariableValue($1);}
	|Expression PLUS Expression {$$=new Val2Expression('+',$1,$3);}
	|Expression MINUS Expression {$$=new Val2Expression('-',$1,$3);}
	|Expression MUL Expression {$$=new Val2Expression('*',$1,$3);}
	|Expression DIV Expression {$$=new Val2Expression('/',$1,$3);}
	|Expression GE Expression {$$=new Val2Expression('L',$1,$3);}
	|Expression GT Expression {$$=new Val2Expression('>',$1,$3);}
	|Expression LE Expression {$$=new Val2Expression('G',$1,$3);}
	|Expression LT Expression {$$=new Val2Expression('<',$1,$3);}
	|Expression SEQUALS Expression {$$=new Val2Expression('=',$1,$3);}
	|Expression NE Expression {$$=new Val2Expression('!',$1,$3);}
	|LPAREN Expression RPAREN {$$=$2;}
	|CompositeMethodCall {$$=$1;}
	|ProcedureCall {$$=$1;}
	|Assignment {$$ = $1;}
        |Return {$$ = $1;}
        |While {$$ = $1;}
        |If {$$ = $1;}
	;

Literal:
	 INTEGER {$$=new LitInt($1);}
	|FLOAT {$$=new LitFloat($1);}
	|BOOLEAN {$$=new LitBool($1);}
	;

CommaSeparated:
         /* empty */ {$$=new CommaSeparated();}
	|IDENTIFIER {$$=new CommaSeparated(); $$->addIdentifier($1);}
	|CommaSeparated COMMA IDENTIFIER {$$->addIdentifier($3);}
  	; 

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
   printf("yyerror : %s %s\n",y,*x);
}

pProgramNode* glob;

int main(void) {
   yydebug = 1;
   glob = new pProgramNode();
   char* errmsg = "error";
   int result = yyparse(glob,&errmsg);
   glob->print(0);
}

