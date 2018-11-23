#pragma once

struct FILE;

int execap(const char *name, const char *arg);

static inline int exec(const char *name)
{
	return execap(name, "");
}

#define stexec(name) create_thread(create_process(exec, name))
