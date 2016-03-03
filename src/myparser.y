%{
#include "Expression.h"
#include "Assignment.h"
#include "If.h"
#include "pReturn.h"
#include "While.h"
#include "CommaSeparated.h"
#include "MethodCall.h"
#include "ExpressionList.h"
#include "LitBool.h"
#include "Literal.h"
#include "LitFloat.h"
#include "LitInt.h"
#include "LitString.h"
#include "pClassDefinition.h"
#include "pProcedureNode.h"
#include "pProgramNode.h"
#include "ProcedureCall.h"
#include "Val2Expression.h"
#include "VariableValue.h"
#include "CodeGenerator.h"
#include "Disassembler.h"
#include "PrintNode.h"
#include "CInterpreter.h"
#include "Extern.h"

#include <stdio.h>

extern char *yytext;
#define YYDEBUG_LEXER_TEXT yytext

%}

%union
{
    char* sval;
    pProgramNode *a_program;
    pClassDefinition *a_class;
    pProcedureNode *a_procedure;
    pProcedureNode *a_method;
    Assignment *an_assignment;
    If *an_if;
    pReturn* a_return;
    While* a_while;
    MethodCall *a_methodcall;
    ProcedureCall *a_procedurecall;
    ExpressionList *an_expressionlist;
    Expression *an_expression;
    Statements *a_statements;
    CommaSeparated *a_comma_separated;
    Literal *a_literal;
    VariableValue *a_variablevalue;
    char* Integer;
    double Double;
    char* Boolean;
    char* String;
    PrintNode *a_printnode;
    Extern *an_extern;
    char* an_estring;
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
%token PRINT
%token EXTERN
%token ESTRING

%nonassoc BLOCK
%nonassoc IDENTIFIER

%left PLUS MINUS
%left MUL DIV
%left NEG

%right POINT

%parse-param {pProgramNode *glob}
%parse-param {char **errmsg}

%type <a_program> Program
%type <an_extern> Extern
%type <a_class> Class
%type <a_procedure> Procedure
%type <a_method> Method
%type <a_program> Highlevelblock
%type <an_assignment> Assignment
%type <an_if> If 
%type <a_return> Return
%type <a_while> While
%type <a_methodcall> MethodCall
%type <a_procedurecall> ProcedureCall
%type <an_expressionlist> ExpressionList
%type <an_expression> Expression
%type <a_statements> BSB
%type <a_statements> Statements
%type <a_comma_separated> CommaSeparated
%type <a_literal> Literal
%type <a_variablevalue> Lhs
%type <Integer> INTEGER
%type <String> FLOAT
%type <Boolean> BOOLEAN
%type <String> STRING
%type <sval> IDENTIFIER
%type <a_printnode> Print
%type <an_estring> ESTRING

%token PROCEDURE
%start Program

%%

Program:
   /* empty */  {$$ =glob;}
   |Program Highlevelblock {$$=glob;}
   ;

Highlevelblock:
    Class { glob->addClass($1);}
   |Procedure { glob->addProcedure($1);}
   |Method { glob->addMethodDefinition($1);}
   |Extern { glob->addExtern($1); }
   ; {$$=glob;}

Procedure:
   PROCEDURE IDENTIFIER LPAREN CommaSeparated RPAREN BSB ; {$$=new pProcedureNode("",$2,$4,$6);}

Class:
   CLASS IDENTIFIER BLOCK CommaSeparated ENDBLOCK
   ; {  $$ = new pClassDefinition($2);}

Extern:
   EXTERN IDENTIFIER ESTRING SEMICOL
   ; { $$ = new Extern($2,$3); }

Method:
   METHOD IDENTIFIER POINT IDENTIFIER LPAREN CommaSeparated RPAREN BSB ; {  $$ = new pProcedureNode($2,$4,$6,$8);}

Statements:
    /* empty */ { $$=new Statements();} 
   |Expression { $$ = new Statements(); $$->addStatement($1); }
   |Statements SEMICOL Expression { $$=$1;$1->addStatement($3); }
   |Statements SEMICOL { $$=$1; }
   ;

BSB:
   BLOCK Statements ENDBLOCK {$$ = $2;}
   ;

Assignment:
   Lhs EQUALS Expression 
   ; { $$ = new Assignment($1,$3);}

Return:
   RETURN Expression { $$ = new pReturn($2); }
  |RETURN { $$ = new pReturn(NULL); }
   ;

While:
   WHILE Expression BSB { $$ = new While($2,$3); }
   ;

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

MethodCall: 
   Expression POINT IDENTIFIER LPAREN ExpressionList RPAREN; { $$ = new MethodCall($1,$3,$5);}

Expression:
    Literal { $$=$1; }
   |IDENTIFIER { $$=new VariableValue($1);}
   |Expression PLUS Expression {$$=new Val2Expression('+',$1,$3);}
   |Expression MINUS Expression {$$=new Val2Expression('-',$1,$3);}
   |Expression MUL Expression {$$=new Val2Expression('*',$1,$3);}
   |Expression DIV Expression {$$=new Val2Expression('/',$1,$3);}
   |Expression GE Expression {$$=new Val2Expression('G',$1,$3);}
   |Expression GT Expression {$$=new Val2Expression('>',$1,$3);}
   |Expression LE Expression {$$=new Val2Expression('L',$1,$3);}
   |Expression LT Expression {$$=new Val2Expression('<',$1,$3);}
   |Expression SEQUALS Expression {$$=new Val2Expression('=',$1,$3);}
   |Expression NE Expression {$$=new Val2Expression('!',$1,$3);}
   |LPAREN Expression RPAREN {$$=$2;}
   |MethodCall {$$=$1;}
   |ProcedureCall {$$=$1;}
   |Assignment {$$ = $1;}
   |Return {$$ = $1;}
   |While {$$ = $1;}
   |If {$$ = $1;}
   |Print {$$ = $1;}
   ;

Print:
   PRINT Expression {$$=new PrintNode($2);}
   ;

Literal:
    FLOAT {$$=new LitFloat($1);}
   |INTEGER {$$=new LitInt($1);}
   |BOOLEAN {$$=new LitBool($1);}
   |STRING {$$=new LitString($1);}
   ;

CommaSeparated:
         /* empty */ {$$=new CommaSeparated();}
   |IDENTIFIER {$$=new CommaSeparated();$$->addIdentifier($1);}
   |CommaSeparated COMMA IDENTIFIER {$$ = $1; $1->addIdentifier($3);}
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

int main(int argc, char* argv[]) {
//
// workaround for a bug in the Eclipse console
//
setvbuf(stdout, NULL, _IONBF, 0);
   extern FILE * yyin;
   if (argc != 2) {
      cout << "Must provide filename as an argument, example " << argv[0]
            << " c:\\\\test\\\\test.src" << endl;
      return -1;
   }
   cout << "Parsing... " << argv[1] << " ... " << endl;
   yyin = fopen(argv[1],"r");
   yydebug = 1;
   glob = new pProgramNode();
   char* errmsg = "error";
   int result = yyparse(glob,&errmsg);
   fclose(yyin);
   glob->print(0);
   CodeGenerator cg;
   cg.start(glob,NULL);
   Disassembler d; 
   d.start(cg.getCodeBuffer(),cg.getHere(),NULL);
   //
   // start interpreting
   //
   CInterpreter i(cg.getCodeBuffer(),NULL);
   i.start();
}


