#include <dlfcn.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libshell.h"

struct module {
	const char *name;
	void *handle;
	int (*func)(struct list **);
};

struct module *modules;
size_t module_num;

static int load_modules(const char **);

int
main(int argc, char *argv[]) {
	const char *init_modules[] = { "frontend/sh", "backend/exec", NULL };
	struct list *l = NULL;
	size_t i;

	if (load_modules(init_modules) != 0)
		errx(1, "Failed to load initial modules");

	while (1) {
		printf("> ");
		for (i = 0; i < module_num; i++) {
			if (modules[i].func(&l) != 0) {
				warnx("module %s failed", modules[i].name);
			}
		}
		free_list(l);
	}

	return 0;
}

static int
load_modules(const char **m) {
	struct module *old_modules = modules;
	char buf[32];
	size_t i, old_module_num = module_num;
	int ret;

	for (i = 0; m[i] != NULL; i++);

	if ((modules = reallocarray(NULL, i, sizeof(struct module))) == NULL) {
		warn("Could not allocate memory for new modules");
		return 1;
	}

	for (i = 0, module_num = 0; m[i] != NULL; i++) {
		modules[i].name = strdup(m[i]);

		ret = snprintf(buf, sizeof(buf), "%s.so", modules[i].name);
		if (ret < 0 || ret >= (int)sizeof(buf)) {
			warnx("module name too long");
			goto err;
		}

		if ((modules[i].handle = dlopen(buf, RTLD_NOW)) == NULL) {
			warnx("Could not open module %s", modules[i].name);
			goto err;
		}

		module_num++;

		if ((modules[i].func = dlsym(modules[i].handle, "libshell_func")) == NULL) {
			warnx("Malformed module %s", modules[i].name);
			goto err;
		}
	}

	if (old_modules != NULL) {
		for (i = 0; i < old_module_num; i++) {
			free((void *)(long)old_modules[i].name);
			dlclose(old_modules[i].handle);
		}
		free(old_modules);
	}

	return 0;

err:
	for (i = 0; i < module_num; i++) {
		dlclose(modules[i].handle);
	}
	free(modules);

	modules = old_modules;
	module_num = old_module_num;
	return 1;
}
