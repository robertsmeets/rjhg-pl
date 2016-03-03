%{
#include "Expression.h"
#include "Assignment.h"
#include "While.h"
#include "If.h"
#include "CommaSeparated.h"
#include "MethodCall.h"
#include "Expression.h"
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
#include "PrintNode.h"
#include "Extern.h"

#include "y.tab.h"

extern "C" int yylex();
%}

blanks          [ \t\n]+
identifier   	[_a-zA-Z]+
float 		[0-9]+\.[0-9]*
integer		[0-9]+
boolean        	true|false
string        	\"[\% \.a-zA-Z0-9]*\"
estring         [BcCsSiIjJlLfdpZv]*[\-\+][BcCsSiIjJlLfdpZv]*

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
"print"     return(PRINT);
"extern"    return(EXTERN);

{boolean} {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(BOOLEAN);
      }

{string} {
            yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(STRING);
      }

{identifier}   {
	    yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(IDENTIFIER);
      }

{estring}   {
	    yylval.sval = malloc(strlen(yytext)+1);
            strncpy(yylval.sval, yytext, strlen(yytext)+1);
            return(ESTRING);
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


%%