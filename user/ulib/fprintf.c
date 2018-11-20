#include <stdio.h>
#include <gprintf.h>
#include <assert.h>
#include <fifo.h>


static FIFO my_fifo;

static void my_fputc(char c)
{
	assert(fifo_workwell(&my_fifo));
	fifo_put(&my_fifo, c);
}

int vfprintf
	( FILE *f
	, const char *fmt
	, va_list ap
	)
{
	fifo_init(&my_fifo);
	int ret = vgprintf(my_fputc, fmt, ap);
	int size = fifo_size(&my_fifo);
	while (size--) {
		fputc(fifo_get(&my_fifo), f);
	}
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
