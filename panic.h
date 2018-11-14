#pragma once

void __attribute__((noreturn)) panic
		( const char *fmt
		, ...
		);

#define assert(x) \
	if (!(x)) { panic("ASSERTION_FAILURE: " #x); } else {}
