#pragma once

void __attribute__((noreturn)) panic
		( const char *fmt
		, ...
		);

#define assert(x) \
	if (!(x)) { panic("assert(" #x ") failed"); } else {}
