#include <stdio.h>
#include <gprintf.h>
#include <assert.h>
#include <fifo.h>


#define PRINT_BUFSIZ BUFSIZ
static char buf[PRINT_BUFSIZ];
static int i;

static void my_buf_putc(char c)
{
	assert();
	buf[i++] = c;
}

int vfprintf
	( FILE *f
	, const char *fmt
	, va_list ap
	)
{
	i = 0;

	int ret = vgprintf(my_buf_putc, fmt, ap);

	my_buf_putc(0);

	fputs(buf, f);

	return ret;
}


int fprintf
	( FILE *f
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vfprintf(f, fmt, ap);
	va_end(ap);
	return res;
}
