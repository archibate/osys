#pragma once

#include "print.h"

#define panic(...) printf("PANIC: " __VA_ARGS__)

#define assert(x) \
	if (!(x)) { panic("assertion failed: " #x) } else {}
