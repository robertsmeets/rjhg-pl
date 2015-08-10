%{
#include "Assignment.h"
#include "Return.h"
#include "While.h"
#include "If.h"
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

#include "y.tab.h"

extern "C" int yylex();
%}

blanks          [ \t\n]+
identifier   	[_a-zA-Z]+
integer		[0-9]+
boolean        	true|false
string        	"[a-zA-Z0-9]*"
float 		[0-9]+.[0-9]*[e[0-9]+]

%%

{blanks}        { /* ignore */ }

"proc"      return(PROCEDURE);
"{"         return(BLOCK);
"}"         return(ENDBLOCK);
"method"    return(METHOD);
"class"     return(CLASS);
"."         return(POINT);
","         return(COMMA);
"="         return(EQUALS);
"=="        return(SEQUALS);
">"         return(GT);
"<"         return(LT);
">="        return(GE);
"<="        return(LE);
"!="        return(NE);
";"         return(SEMICOL);
"("         return(LPAREN);
")"         return(RPAREN);
"+"         return(PLUS);
"-"         return(MINUS);
"*"         return(MUL);
"/"         return(DIV);
"return"    return(RETURN);
"if"        return(IF);
"else"      return(ELSE);
"while"     return(WHILE);

{identifier}   {
	    yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
	    printf("ID<%s>\n",yytext);
            return(IDENTIFIER);
      }

{float}   {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(FLOAT);
      }


{integer}   {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(INTEGER);
      }


{string} {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(STRING);
      }

{boolean} {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(BOOLEAN);
      }

%%
