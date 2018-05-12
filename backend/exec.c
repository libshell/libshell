#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

static int run_command_internal(const struct command *, const int[]);

int
libshell_func(const struct list **l) {
	return run_list(*l, default_fd);
}

int
run_list(const struct list *l, const int fd[]) {
	size_t i;
	int last_exit_status, ret, *statuses = NULL;

	for (i = 0; i < l->n_pipelines; i++) {
		if (i != 0) {
			if ((l->separators[i - 1] == LSEP_AND && last_exit_status != 0) ||
			    (l->separators[i - 1] == LSEP_OR  && last_exit_status == 0))
				continue;
		}

		if ((statuses = reallocarray(statuses, l->pipelines[i]->n_commands, sizeof(int))) == NULL) {
			warn("reallocarray");
			ret = 1;
			goto exit;
		}
		if ((ret = run_pipeline(l->pipelines[i], fd, statuses, &last_exit_status)) != 0)
			goto exit;
	}

exit:
	free(statuses);
	return ret;
}

int
run_pipeline(const struct pipeline *p, const int fd[], int *statuses, int *pipeline_status) {
	size_t i;
	pid_t pid, *pids;
	int command_fds[3], pipe_fds[2], ret = 0, status;

	if ((pids = reallocarray(NULL, p->n_commands, sizeof(pid_t))) == NULL) {
		warn("reallocarray");
		ret = 1;
		goto exit;
	}

	for (i = 0; i < p->n_commands; i++) {
		if (i == 0)
			command_fds[0] = fd[0];
		else {
			command_fds[0] = pipe_fds[0];
			close(pipe_fds[1]);
		}

		if (i == p->n_commands - 1)
			command_fds[1] = fd[1];
		else {
			if (pipe(pipe_fds) != 0) {
				warn("pipe");
				ret = 1;
				goto exit;
			}
			command_fds[1] = pipe_fds[1];
		}

		command_fds[2] = fd[2];

		if ((ret = run_command(p->commands[i], command_fds, &pids[i])) != 0)
			goto exit;
	}

	while (1) {
		do {
			pid = waitpid(WAIT_MYPGRP, &status, WUNTRACED);
		} while (WIFSTOPPED(status) || (pid == -1 && errno == EINTR));

		if (pid == -1 && errno == ECHILD)
			break;

		for (i = 0; i < p->n_commands && pids[i] != pid; i++);

		if (statuses != NULL)
			statuses[i] = status;
	}

	*pipeline_status = WEXITSTATUS(statuses[p->n_commands - 1]);
	if (p->negate)
		*pipeline_status = !*pipeline_status;

exit:
	free(pids);
	return ret;
}

int
run_command(const struct command *c, const int fd[], pid_t *pidp) {
	switch (*pidp = fork()) {
		case 0:
			run_command_internal(c, fd);
		case -1:
			return 1;
	}

	return 0;
}

static int
run_command_internal(const struct command *c, const int fd[]) {
	if (fd[0] >= 0 && fd[0] != STDIN_FILENO) {
		if (dup2(fd[0], STDIN_FILENO) < 0)
			_exit(1);
	}

	if (fd[1] >= 0 && fd[1] != STDOUT_FILENO) {
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			_exit(1);
	}

	if(fd[2] >= 0 && fd[2] != STDERR_FILENO) {
		if (dup2(fd[2], STDERR_FILENO) < 0)
			_exit(1);
	}

	execvp(c->args[0], c->args);

	exit(1);
}
