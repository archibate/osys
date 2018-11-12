#pragma once


#include <stdarg.h>


int vgprintf
	( void (*putch)(char)
	, const char *fmt
	, va_list ap
	);

int gprintf
	( void (*putch)(char)
	, const char *fmt
	, ...
	);
