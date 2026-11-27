%{
#include "Expression.h"
#include "Assignment.h"
#include "If.h"
#include "ReturnNode.h"
#include "While.h"
#include "CommaSeparated.h"
#include "MethodCall.h"
#include "ExpressionList.h"
#include "LitBool.h"
#include "LitNull.h"
#include "Literal.h"
#include "LitFloat.h"
#include "LitInt.h"
#include "LitString.h"
#include "ClassDefinition.h"
#include "ProcedureNode.h"
#include "ProgramNode.h"
#include "ProcedureCall.h"
#include "NotExpression.h"
#include "Val2Expression.h"
#include "VariableValue.h"
#include "CodeGenerator.h"
#include "Disassembler.h"
#include "PrintNode.h"
#include "CInterpreter.h"
#include "Extern.h"
#include "Self.h"
#include "SelfTest.h"
#include "Comments.h"
#include "Runner.h"
#include "SelfStart.h"
#include "FileCreator.h"
#include "Interactive.h"
#include "ast.h"
#include <stdio.h>
#include <iostream>
#include <stdbool.h>

extern int line_num;
extern char *yytext;

using namespace std;

extern "C"
{
    int yylex(void);
    int yywrap()
    {
        return 1;
    }
    int S_start(char*);
}
void yyerror(const char* y) { printf("yyerror : line_num %d\n",line_num); exit(-1); }


// #define YYDEBUG 0
#define YYDEBUG_LEXER_TEXT yytext
%}
%union
{
    char* sval;
    ProgramNode *a_program;
    ClassDefinition *a_class;
    ProcedureNode *a_procedure;
    ProcedureNode *a_method;
    Assignment *an_assignment;
    If *an_if;
    AST* a_return;
    While* a_while;
    MethodCall *a_methodcall;
    ProcedureCall *a_procedurecall;
    ExpressionList *an_expressionlist;
    Expression *an_expression;
    Statements *a_statements;
    CommaSeparated *a_comma_separated;
    AST *a_literal;
    VariableValue *a_variablevalue;
    char* Integer;
    double Double;
    char* Boolean;
    char* String;
    PrintNode *a_printnode;
    Extern *an_extern;
    char* an_estring;
    void* nullid;
    AST* ast;
};

%token IDENTIFIER
%token SELF
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
%token AND
%token OR
%token NOT
%token MOD
%token LPAREN
%token RPAREN
%token LBRACKET
%token RBRACKET
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
%token NULLID

%nonassoc BLOCK
%nonassoc IDENTIFIER

%left PLUS MINUS
%left MUL DIV
%left NEG

%right POINT

/*

%parse-param {ProgramNode *glob}
%parse-param {char **errmsg}


*/

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
%type <an_expression> Statement
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
%type <nullid> NULLID

%glr-parser
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
   |Statements {glob->addStatements($1); }
    {$$=glob;};

Procedure:
   PROCEDURE IDENTIFIER LPAREN CommaSeparated RPAREN BSB  {$$=new ProcedureNode("",$2,$4,$6);} ;

Class:
   CLASS IDENTIFIER BLOCK CommaSeparated ENDBLOCK
    {  $$ = new ClassDefinition($2,$4);};

Extern:
   EXTERN IDENTIFIER ESTRING SEMICOL
    { $$ = new Extern($2,$3); };

Method:
   METHOD IDENTIFIER POINT IDENTIFIER LPAREN CommaSeparated RPAREN BSB  {  $$ = new ProcedureNode($2,$4,$6,$8);};

Statements:
   Statement {
      AST *term = (AST*) malloc(sizeof(struct AST));
      term -> operand = AST_STATEMENTS;
      term -> two = $1;
      $$ = (Statements*) term;}
   |Statements SEMICOL Statement {
      AST* term = (AST*) $1;
      int size = term->data.intValue + 1;
      AST* newloc = (AST*) realloc(term, sizeof(struct AST)  + size * sizeof(void*));
      newloc -> data.intValue = size;
      newloc -> list[size-1] = $3;
      $$ = (Statements*) newloc; }
   |Statements SEMICOL { $$=$1; }
   ;

BSB:
   BLOCK Statements ENDBLOCK {$$ = $2;}
   ;

Assignment:
   Lhs EQUALS Expression
    { $$ = new Assignment($1,$3);};

Return:
   RETURN Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
      term -> operand = AST_RETURNNODE;
      term -> two = $2;
      $$ = term;}
  |RETURN {
      AST *term = (AST*) malloc(sizeof(struct AST));
      term -> operand = AST_RETURNNODE;
      term -> two = 0;
      $$ = term;}
   ;

While:
   WHILE Expression BSB { $$ = new While($2,$3); }
   ;

If:
   IF Expression BSB { $$ = new If($2,$3,NULL); }
  |IF Expression BSB ELSE BSB { $$ = new If($2,$3,$5); }
   ;

Lhs:
   IDENTIFIER  {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VARIABLEVALUE;
             term -> two = $1;
             $$ = (VariableValue*) term;}

ProcedureCall:
   IDENTIFIER LPAREN ExpressionList RPAREN  {
      AST *term = (AST*) malloc(sizeof(struct AST));
      term -> operand = AST_PROCEDURECALL;
      term -> two = $1;
      term -> three = $3;
      $$ = (ProcedureCall*) term;}

ExpressionList:
    /* empty */ {
      AST *term = (AST*) malloc(sizeof(struct AST) );
             term -> operand = AST_EXPRESSIONLIST;
             term -> data.intValue = 0;
             $$ = (ExpressionList*) term;}
   | Expression {
      AST *term = (AST*) malloc(sizeof(struct AST) + sizeof(void*));
             term -> operand = AST_EXPRESSIONLIST;
             term -> data.intValue = 1;
             term -> list[0] = $1;
             $$ = (ExpressionList*) term;}
   | ExpressionList COMMA Expression  {
             AST* term = (AST*) $1;
             int size = term->data.intValue + 1;
             AST* newloc = (AST*) realloc(term, sizeof(struct AST)  + size * sizeof(void*));
             newloc -> data.intValue = size;
             newloc -> list[size-1] = $3;
             $$ = (ExpressionList*) newloc; }
   ;

MethodCall:
   Expression POINT IDENTIFIER LPAREN ExpressionList RPAREN {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_METHODCALL;
             term -> two = $1;
             term -> three = $3;
             term -> four = $5;
             $$ = (MethodCall*) term;}

Expression:
    Literal { $$=(Expression*) $1; }
   |IDENTIFIER {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VARIABLEVALUE;
             term -> three = $1;
             $$ = (Expression*) term;}
   |Expression PLUS Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '+';
             term -> three = $1;
             term -> four = $3;
             $$ = (Expression*) term;}
   |Expression MINUS Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '-';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression MUL Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '*';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression DIV Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '/';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression GE Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) 'G';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression GT Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '>';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression LE Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) 'L';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression LT Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '>';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression AND Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '&';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression MOD Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '%';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |Expression OR Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '|';
             term -> three = $1;
             term -> four = $3;
             $$ = (Literal*) term;}
   |NOT Expression {
      AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*) '!';
             term -> three = $2;
             $$ = (Literal*) term;}
   |Expression SEQUALS Expression {
             AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*)'=';
             term -> three = $1;
             term -> four = $3;
             $$ = (MethodCall*) term;}
   |Expression NE Expression {
             AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*)'N';
             term -> three = $1;
             term -> four = $3;
             $$ = (MethodCall*) term;}
   |LPAREN Expression RPAREN {$$=$2;}
   |Expression LBRACKET Expression RBRACKET  {
             AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_VAL2EXPRESSION;
             term -> two = (void*)'I';
             term -> three = $1;
             term -> four = $3;
             $$ = (MethodCall*) term;}
   |MethodCall {$$=$1;}
   |ProcedureCall {$$=$1;}
   |SELF {
             AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_SELF;
             $$ = (MethodCall*) term;}
   ;

Statement:
    Assignment {$$ = $1;}
   |Return {$$ = (Expression*) $1;}
   |While {$$ = $1;}
   |If {$$ = $1;}
   |Print {$$ = $1;}
   |MethodCall {$$=$1;}
   |ProcedureCall {$$=$1;}
   ;


Print:
   PRINT Expression {
             AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_PRINTNODE;
             term -> two = $2;
             $$ = (PrintNode*) term;}
   ;


Literal:
    FLOAT {  AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_LITFLOAT ;
             term -> data.intValue = atof($1);
             $$ = term;}
   |BOOLEAN {AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_LITBOOL ;
             term -> data.boolValue = litboolconversion($1);
             $$ = term;}
   |INTEGER {AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_LITINT ;
             term -> data.intValue = atoi($1);
             $$ = term;}
   |STRING {AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_LITSTRING ;
             term -> data.ptrValue = $1;
             $$ = term;}
   |NULLID {AST *term = (AST*) malloc(sizeof(struct AST));
             term -> operand = AST_LITNULL ;
             $$ = term;}
   ;

CommaSeparated:
         /* empty */ {$$=new CommaSeparated();}
   |IDENTIFIER {$$=new CommaSeparated();$$->addIdentifier($1);}
   |CommaSeparated COMMA IDENTIFIER {$$ = $1; $1->addIdentifier($3);}
     ;


%%

/**
 *
 * main entry point
 *
 */
int main(int argc, char* argv[]) {
   //
   // workaround for a bug in the Eclipse console
   //
   setvbuf(stdout, NULL, _IONBF, 0);
   GC_INIT();
   //yydebug=1;
   if (argc == 2)
   {
      if (strcmp(argv[1],"t") == 0)
      {
         SelfTest st;
         st.run();
      }
      else
      {
         Runner runner;
         return runner.compile_run(argv[1],true);
      }
   }
   else if (argc == 3)
   {
      if (strcmp(argv[1],"c") == 0)
      {
         Runner runner;
         CodeGenerator* cg = new CodeGenerator();
         runner.compile_with_system(cg,argv[2],false);
         FileCreator fc;
         fc.start(argv[2],cg->getCodeBuffer(),cg->getHere());
      }
   }
   else
   {
      int result = S_start(argv[0]);
      if (result==-1)
      {
         Interactive i;
         i.run();
      }
   }
}
