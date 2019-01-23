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
	struct list *list;
	struct pipeline *pipeline;
	struct sublist *sublist;
}

%type <command> command
%type <pipeline> pipeline
%type <list> list
%type <sublist> sublist
%type <b> sublistend

%%

list:
	  /* empty */				{ $$ = NULL; return_list = $$; }
	| list sublist sublistend		{ $$ = add_sublist($1, $2, $3); return_list = $$; }

sublistend:
	  TOK_SEMI			{ $$ = false; }
	| TOK_NL			{ $$ = false; }
	| '\0'				{ $$ = false; }
	| TOK_AMPER			{ $$ = true; }

sublist:
	  pipeline			{ $$ = add_pipeline(NULL, $1, 0); }
	| sublist TOK_DAMPER pipeline	{ $$ = add_pipeline($1, $3, LSEP_AND); }
	| sublist TOK_DPIPE pipeline	{ $$ = add_pipeline($1, $3, LSEP_OR); }

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
