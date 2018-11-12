#pragma once


#include <stdarg.h>


int vprintf
	( const char *fmt
	, va_list ap
	);

int printf
	( const char *fmt
	, ...
	);
