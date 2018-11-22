#pragma once

struct FILE;

int stexecv1(const char *name, const char *arg);
#define stexec(name) stexecv1((name), "")
