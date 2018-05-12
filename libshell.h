#ifndef _LIBSHELL_H_
#define _LIBSHELL_H_

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>

struct command {
	char **args;
	size_t n_args;
};

struct pipeline {
	struct command **commands;
	size_t n_commands;
	bool negate;
};

struct list {
	struct pipeline **pipelines;
	char *separators;
	size_t n_pipelines;
	bool async;
};

#define LSEP_AND 0
#define LSEP_OR 1

const int default_fd[3];

struct command *add_arg(struct command *, char *);
struct pipeline *add_command(struct pipeline *, struct command *);
struct list *add_pipeline(struct list *, struct pipeline *, int sep);
void free_command(struct command *);
void free_pipeline(struct pipeline *);
void free_list(struct list *);

#endif
