#pragma once

#include <print.h>

#define panic(...) do { \
	printf("PANIC: " __VA_ARGS__); \
	asm volatile ("cli; hlt"); \
	__builtin_unreachable(); \
} while (0)

#define assert(x) \
	if (!(x)) { panic("assertion failed: " #x) } else {}
