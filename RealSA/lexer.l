%{
//header section insert into output
#include "../inc/reg_lang.h"
#include "parser.h"
#include <stdio.h>
%}

%option outfile="tmp/lexer.c" header-file="tmp/lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

ASSIGN  ":="
PLUS    "+"
TIMES   "*"
EQUALS  "="
NUM     "-"?[0-9]+
SC      ";"
COL     ":"
GOTO    "goto"
INPUT   "input"
OUTPUT  "output"
IF      "if"

IDEN    [a-zA-Z_][a-zA-Z0-9]*
LPAREN  "("
RPAREN  ")"

WS	[ \t\r\n]+

/* begin tokenizing*/
%%
{WS} { /*ignore white spaces, we are kind*/ }
{ASSIGN}    {return _ASSIGN;}
{PLUS}      {return _PLUS;}
{TIMES}     {return _TIMES;}
{EQUALS}    {return _EQUALS;}
{NUM}       {yylval->str = (char*)malloc(sizeof(char)*(1+ strlen(yytext))); strcpy(yylval->str, yytext); return _NUM;}
{SC}        {return _SC;}
{COL}       {return _COL;}
{GOTO}      {return _GOTO;}
{INPUT}     {return _INPUT;}
{OUTPUT}    {return _OUTPUT;}
{IF}        {return _IF;}
{LPAREN}    {return _LPAREN;}
{RPAREN}    {return _RPAREN;}
{IDEN}      {yylval->str = (char*)malloc(sizeof(char)*(1+ strlen(yytext))); strcpy(yylval->str, yytext); return _IDEN;}

. { printf("ERROR! Unknown input  %s\n", yytext);/*error */ }

%%

