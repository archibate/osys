#pragma once

struct FILE;

int execap(const char *name, const char *arg);

static inline int exec(const char *name)
{
	return execap(name, "");
}

#define stexec(name) add_task(create_process_ex(name, exec, name))
