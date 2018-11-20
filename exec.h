#pragma once

struct FILE;

int exec(const char *name);
int stexec(const char *name);
int exec_f(struct FILE *f);
