#include <err.h>
#include <stdlib.h>

#include "libshell.h"

const int default_fd[3] = { 0, 1, 2 };

struct command *
add_arg(struct command *c, char *arg) {
	if (c == NULL) {
		if ((c = calloc(1, sizeof(struct command))) == NULL)
			err(1, "malloc");
	}

	c->n_args++;
	if ((c->args = reallocarray(c->args, c->n_args + 1, sizeof(char *))) == NULL)
		err(1, "reallocarray");

	c->args[c->n_args - 1] = arg;
	c->args[c->n_args] = NULL;

	return c;
}

struct pipeline *
add_command(struct pipeline *p, struct command *c) {
	if (p == NULL) {
		if ((p = calloc(1, sizeof(struct pipeline))) == NULL)
			err(1, "malloc");
	}

	p->n_commands++;
	if ((p->commands = reallocarray(p->commands, p->n_commands, sizeof(struct command *))) == NULL)
		err(1, "reallocarray");

	p->commands[p->n_commands - 1] = c;

	return p;
}

struct list *
add_pipeline(struct list *l, struct pipeline *p, int sep) {
	if (l == NULL) {
		if ((l = calloc(1, sizeof(struct list))) == NULL)
			err(1, "malloc");
	}

	l->n_pipelines++;

	if ((l->pipelines = reallocarray(l->pipelines, l->n_pipelines, sizeof(struct pipeline *))) == NULL)
		err(1, "reallocarray");

	if ((l->separators = reallocarray(l->separators, l->n_pipelines, sizeof(char *))) == NULL)
		err(1, "reallocarray");

	l->pipelines[l->n_pipelines - 1] = p;
	if (l->n_pipelines >= 2)
		l->separators[l->n_pipelines - 2] = sep;

	return l;

}

void
free_command(struct command *c) {
	size_t i;

	if (c == NULL)
		return;

	for (i = 0; i < c->n_args; i++)
		free(c->args[i]);

	free(c->args);
	free(c);
}

void
free_pipeline(struct pipeline *p) {
	size_t i;

	if (p == NULL)
		return;

	for (i = 0; i < p->n_commands; i++)
		free_command(p->commands[i]);

	free(p->commands);
	free(p);
}

void
free_list(struct list *l) {
	size_t i;

	if (l == NULL)
		return;

	for (i = 0; i < l->n_pipelines; i++)
		free_pipeline(l->pipelines[i]);

	free(l->pipelines);
	free(l->separators);
	free(l);
}
