#pragma once

#include <stdarg.h>

int vsprintf
	( char *buf
	, const char *fmt
	, va_list ap
	);

int sprintf
	( char *buf
	, const char *fmt
	, ...
	);
