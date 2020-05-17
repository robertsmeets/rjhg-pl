%{
#include "Expression.h"
#include "Assignment.h"
#include "While.h"
#include "If.h"
#include "CommaSeparated.h"
#include "MethodCall.h"
#include "ExpressionList.h"
#include "LitBool.h"
#include "Literal.h"
#include "LitFloat.h"
#include "LitInt.h"
#include "LitString.h"
#include "ClassDefinition.h"
#include "ProcedureNode.h"
#include "ProgramNode.h"
#include "ProcedureCall.h"
#include "PrintNode.h"
#include "Extern.h"

#include "y.tab.h"

#undef __cplusplus

int line_num = 1;
char* strbuf = (char*)GC_MALLOC(1024);
int lenstring = 0;
extern "C" int yylex();
%}

%option noyywrap
%x string   

blanks          [ \t]+
identifier   	[_a-zA-Z]+
float 		[0-9]+\.[0-9]*
integer		-?[0-9]+
boolean        	true|false
estring         [BcCsSiIjJlLfdpZv]+[\-\+][BcCsSiIjJlLfdpZv]+
string        	\"[\% \.a-zA-Z0-9\,\;\!\{\}\(\)\#\\\>\<]*\"\\

%%
\"                  { BEGIN string; strbuf[0] = '\0'; lenstring = 0; }
<string>[^\\"\n]*   { int len = strlen(yytext); strncpy(strbuf+lenstring,yytext,len);lenstring+=len; strbuf[lenstring] = '\0'; }
<string>\\n         { strbuf[lenstring] = '\n'; lenstring++; strbuf[lenstring]='\0'; }
<string>\\t         { strbuf[lenstring] = '\t'; lenstring++; strbuf[lenstring]='\0'; }
<string>\\\\        { strbuf[lenstring] = '\\'; lenstring++; strbuf[lenstring]='\0'; }
<string>\\\"        { strbuf[lenstring] = yytext[1]; lenstring++; strbuf[lenstring] = '\0'; }
<string>\"          { yylval.sval = (char*)GC_MALLOC(lenstring+1); strncpy(yylval.sval,strbuf,lenstring+1); BEGIN 0; return STRING; }
<string>\\.         { printf("bogus escape '%s' in string\n", yytext);exit(-1); }
<string>\n          { printf("newline in string\n"); exit(-1); }

{blanks}        { /* ignore */ }
\n             { ++line_num; }

"self"      return(SELF);
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
"!"         return(NOT);
"&&"        return(AND);
"||"        return(OR);
"%"         return(MOD);
";"         return(SEMICOL);
"("         return(LPAREN);
")"         return(RPAREN);
\[          return(LBRACKET);
\]          return(RBRACKET);
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
"null"      return(NULLID);

{boolean} {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(BOOLEAN);
      }

{string} {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(STRING);
      }

{identifier}   {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(IDENTIFIER);
      }

{estring}   {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(ESTRING);
      }

{float}   {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(FLOAT);
      }

{integer}   {
            int len = strlen(yytext) + 1;
            yylval.sval = (char*)GC_MALLOC(len);
            strncpy(yylval.sval, yytext, len);
            return(INTEGER);
      }


%%
