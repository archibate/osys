#include <gprintf.h>


static char *my_buf;
static int i;

static void my_buf_putc(char c)
{
	my_buf[i++] = c;
}

int vsprintf
	( char *buf
	, const char *fmt
	, va_list ap
	)
{
	my_buf = buf;
	i = 0;

	int ret = vgprintf(my_buf_putc, fmt, ap);

	my_buf_putc(0);

	return ret;
}


int sprintf
	( char *buf
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vsprintf(buf, fmt, ap);
	va_end(ap);
	return res;
}
