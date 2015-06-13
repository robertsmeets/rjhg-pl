%{
#include "classes.h"
#include "y.tab.h"
extern "C" int yylex();
%}

blanks          [ \t\n]+
identifier   [_a-zA-Z]+
integer		[0-9]+
string        "[a-z]*"
float 		[0-9]+.[0-9]*[e[0-9]+]
boolean        true|false

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
";"         return(SEMICOL);
"("         return(LPAREN);
")"         return(RPAREN);
"+"         return(PLUS);
"-"         return(MINUS);
"*"         return(MUL);
"/"         return(DIV);

{identifier}   {
            yylval.sval = malloc(strlen(yytext));
            strncpy(yylval.sval, yytext, strlen(yytext));
            return(IDENTIFIER);
      }


{float}   {
            yylval.sval = malloc(strlen(yytext));
            strncpy(yylval.sval, yytext, strlen(yytext));
            return(FLOAT);
      }


{integer}   {
            yylval.sval = malloc(strlen(yytext));
            strncpy(yylval.sval, yytext, strlen(yytext));
            return(INTEGER);
      }


{string} {
            yylval.sval = malloc(strlen(yytext));
            strncpy(yylval.sval, yytext, strlen(yytext));
            return(STRING);
      }

{boolean} {
            yylval.sval = malloc(strlen(yytext));
            strncpy(yylval.sval, yytext, strlen(yytext));
            return(BOOLEAN);
      }

%%
class pProgramNode;
