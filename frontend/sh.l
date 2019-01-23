%{
#include <stdbool.h>

#include "sh.h"
%}

%option noyywrap

%%

;	return TOK_SEMI;

\n	return TOK_NL;

&	return TOK_AMPER;

&&	return TOK_DAMPER;

\|\|	return TOK_DPIPE;

\|	return TOK_PIPE;

[ \t]+	;

[^;&| \\\t\n]+	yylval.string = strdup(yytext); return STRING;
