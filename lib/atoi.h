#pragma once


unsigned long strtol
	( const char *s
	, const char **endp
	, int base
	);

#define atoi(s) strtol((s), 0, 10)
