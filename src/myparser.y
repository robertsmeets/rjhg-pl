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

#include <stdio.h>
#include <iostream>

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
    ReturnNode* a_return;
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
    void* nullid;
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
    /* empty */ { $$=new Statements();} 
   |Statement { $$ = new Statements(); $$->addStatement($1); }
   |Statements SEMICOL Statement { $$=$1;$1->addStatement($3); }
   |Statements SEMICOL { $$=$1; }
   ;

BSB:
   BLOCK Statements ENDBLOCK {$$ = $2;}
   ;

Assignment:
   Lhs EQUALS Expression 
    { $$ = new Assignment($1,$3);};

Return:
   RETURN Expression { $$ = new ReturnNode($2); }
  |RETURN { $$ = new ReturnNode(NULL); }
   ;

While:
   WHILE Expression BSB { $$ = new While($2,$3); }
   ;

If:
   IF Expression BSB { $$ = new If($2,$3,NULL); }
  |IF Expression BSB ELSE BSB { $$ = new If($2,$3,$5); }
   ;

Lhs:
   IDENTIFIER  { $$ = new VariableValue($1);};

ProcedureCall:
   IDENTIFIER LPAREN ExpressionList RPAREN  { $$=new ProcedureCall($1,$3); };

ExpressionList:
    /* empty */ {$$ = new ExpressionList();}
   | Expression { $$ = new ExpressionList();$$->addExpression($1);}
   | ExpressionList COMMA Expression  { $$ = $1; $1->addExpression($3); }
   ;

MethodCall: 
   Expression POINT IDENTIFIER LPAREN ExpressionList RPAREN { $$ = new MethodCall($1,$3,$5);};

Expression:
    Literal { $$=$1; }
   |IDENTIFIER { $$=new VariableValue($1); }
   |Expression PLUS Expression {$$=new Val2Expression('+',$1,$3);}
   |Expression MINUS Expression {$$=new Val2Expression('-',$1,$3);}
   |Expression MUL Expression {$$=new Val2Expression('*',$1,$3);}
   |Expression DIV Expression {$$=new Val2Expression('/',$1,$3);}
   |Expression GE Expression {$$=new Val2Expression('G',$1,$3);}
   |Expression GT Expression {$$=new Val2Expression('>',$1,$3);}
   |Expression LE Expression {$$=new Val2Expression('L',$1,$3);}
   |Expression LT Expression {$$=new Val2Expression('<',$1,$3);}
   |Expression AND Expression {$$=new Val2Expression('A',$1,$3);}
   |Expression MOD Expression {$$=new Val2Expression('M',$1,$3);}
   |Expression OR Expression {$$=new Val2Expression('O',$1,$3);}
   |NOT Expression {$$=new NotExpression($2);}
   |Expression SEQUALS Expression {$$=new Val2Expression('=',$1,$3);}
   |Expression NE Expression {$$=new Val2Expression('N',$1,$3);}
   |LPAREN Expression RPAREN {$$=$2;}
   |Expression LBRACKET Expression RBRACKET  {$$=new Val2Expression('I',$1,$3);}
   |MethodCall {$$=$1;}
   |ProcedureCall {$$=$1;}
   |SELF { $$ = new Self(); }
   ;

Statement:
    Assignment {$$ = $1;}
   |Return {$$ = $1;}
   |While {$$ = $1;}
   |If {$$ = $1;}
   |Print {$$ = $1;}
   |MethodCall {$$=$1;}
   |ProcedureCall {$$=$1;}
   ;


Print:
   PRINT Expression {$$=new PrintNode($2);}
   ;

Literal:
    FLOAT {$$=new LitFloat($1);}
   |INTEGER {$$=new LitInt($1);}
   |BOOLEAN {$$=new LitBool($1);}
   |STRING {$$=new LitString($1);}
   |NULLID {$$=new LitNull();}
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
         string filename = argv[1];
         return runner.compile_run(filename,true);
      }
   }
   else if (argc == 3)
   {
      if (strcmp(argv[1],"c") == 0)
      {
         Runner runner;
         CodeGenerator* cg = new CodeGenerator();
         runner.compile_with_system(cg,argv[2],true);
         FileCreator fc;
         fc.start(argv[0],cg->getCodeBuffer(),cg->getHere());
      }
   }
   else
   {
      SelfStart s;
      int result = s.start(argv[0]); 
      if (!result)
      {
         printf("Must provide filename as an argument, example %s c:\\\\test\\\\test.src\n",argv[0]);
         return result;
      }
   }
}
