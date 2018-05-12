%{
#include <err.h>
#include <stddef.h>
#include <string.h>

#include "../libshell.h"

static struct list *return_list;

int yylex();

int frontend_sh(struct list **);
int yyerror(const char *);
%}

%token TOK_AMPER
%token TOK_DAMPER
%token TOK_DPIPE
%token TOK_NL
%token TOK_PIPE
%token TOK_SEMI
%token <string> STRING

%union {
	bool b;
	char *string;
	struct command *command;
	struct pipeline *pipeline;
	struct list *list;
}

%type <command> command
%type <pipeline> pipeline
%type <list> list
%type <b> listend

%%

lists:
	  /* empty */
	| lists list listend		{ return_list = $2; return_list->async = $3; YYACCEPT; }

listend:
	  TOK_SEMI			{ $$ = false; }
	| TOK_NL			{ $$ = false; }
	| '\0'				{ $$ = false; }
	| TOK_AMPER			{ $$ = true; }

list:
	  pipeline			{ $$ = add_pipeline(NULL, $1, 0); }
	| list TOK_DAMPER pipeline	{ $$ = add_pipeline($1, $3, LSEP_AND); }
	| list TOK_DPIPE pipeline	{ $$ = add_pipeline($1, $3, LSEP_OR); }

pipeline:
	  command			{ $$ = add_command(NULL, $1); }
	| pipeline TOK_PIPE command	{ $$ = add_command($1, $3); }

command:
	  STRING			{ $$ = add_arg(NULL, $1); }
	| command STRING		{ $$ = add_arg($1, $2); }

%%

int
libshell_func(struct list **l) {
	int r;

	r = yyparse();
	*l = return_list;
	return r;
}

int
yyerror(const char *s) {
	warnx("%s\n", s);
	return 0;
}
