#include "../libshell.h"

int run_command(const struct command *, const int[], pid_t *);
int run_pipeline(const struct pipeline *, const int[], int*, int*);
int run_sublist(const struct sublist *, const int[]);
int run_list(const struct list *, const int[]);
