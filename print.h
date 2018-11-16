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


#define    C_NORM	"\033[0m"
#define    C_EMPH	"\033[1m"

#define    C_WHITE	"\033[37m"
#define    C_GREY	"\033[30m"
#define    C_RED	"\033[31m"
#define    C_GREEN	"\033[32m"
#define    C_YELLOW	"\033[33m"
#define    C_BLUE	"\033[34m"
#define    C_PURPLE	"\033[35m"
#define    C_CYAN	"\033[36m"
#define    C_WHITE	"\033[37m"


#ifdef PRRDTSC
#include <rdtsc.h>
#define prrdtsc(x) printf(x "%d\n", (rdtsc() >> 8) & 0xffff)
#else
#define prrdtsc(x)
#endif
