#include <serial.h>
#include <gprintf.h>
#include <print.h>


int putchar(char c)
{
	serial_putc(c);
	return 0;
}


int vprintf
	( const char *fmt
	, va_list ap
	)
{
	return vgprintf(serial_putc, fmt, ap);
}


int printf
	( const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vprintf(fmt, ap);
	va_end(ap);
	return res;
}
