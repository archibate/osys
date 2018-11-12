#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <div.h>
#include <atoi.h>


static
void _geutoa
	( void (*putch)(char)
	, const char *digsel
	, unsigned long u
	, unsigned int base
	, unsigned long len
	)
{
	char buf[len], *p = buf + len;
	div_t d;
	do {
		d = div(u, base);
		*--p = digsel[d.rem];
		u = d.quot;
	} while (p > buf);
	while (p < buf + len)
		putch(*p++);
}


static
unsigned long geusslen
	( unsigned long u
	, unsigned int base
	)
{
	unsigned long len = 0;
	do {
		len++;
	} while ((u /= base));
	return len;
}


static
unsigned long gputs
	( void (*putch)(char)
	, const char *s
	)
{
	unsigned long len;
	char c;
	while ((c = *s++))
		len++, putch(c);
	return len;
}


static inline
long get_lon_arg
	( va_list *pap
	, int lon
	)
{
	if (!lon)
		return va_arg(*pap, int);
	else
		return va_arg(*pap, long);
}


static
void gputtimes
	( void (*putch)(char)
	, int times
	, char ch
	)
{
	while (times-- > 0)
		putch(ch);
}


// reference: https://baike.so.com/doc/5432134-5670428.html
int vgprintf
	( void (*putch)(char)
	, const char *fmt
	, va_list ap
	)
{
	char c;

	while ((c = *fmt++)) {
		if (c != '%') {
			putch(c);
			continue;
		}

		const char *s = NULL;
		unsigned long x = 0;
		long l = 0;
		int lon = 0;
		const char *digsel = "01234567890abcdef";
		int base = 10;
		int algnr = 0;
		char possign = 0;
		char prefill = ' ';
		int sharp = 0, doted = 0;
		int rightalg = 0, len = 0;
again:
		switch (c = *fmt++) {
		case 0:
			goto break_while;

		default:
			if ('1' <= c && c <= '9') {
				algnr = strtol(fmt - 1, &fmt, 10);
			} // TODO: on doted = 1??
			goto again;

		case '*':
			algnr = va_arg(ap, int);
			goto again;

		case '%':
			putch(c);
			break;

		case 'l':
			lon++;
			goto again;

		case '.':
			doted = 1; //
			goto again;

		case '#':
			sharp = 1;
			goto again;

		case '0':
			prefill = c;
			goto again;

		case ' ':
		case '+':
			possign = c;
			goto again;

		case '-':
			rightalg = 1;

		case 'c':
			do {
				c = va_arg(ap, int);
				putch(c);
			} while (0);
			break;

		case 's':
			s = va_arg(ap, const char *);
			if (algnr && !rightalg) {
				len = strlen(s);
				gputtimes(putch, algnr - len, prefill);
			}
			len = gputs(putch, s);
			if (algnr && rightalg) {
				gputtimes(putch, algnr - len, prefill);
			}
			break;

		case 'i':
		case 'd':
			l = get_lon_arg(&ap, lon);
			if (l < 0) {
				l = -l;
				putch('-');
			} else if (possign) {
				putch(possign);
			}
			x = l;
			goto utos;

		case 'o':
			base = 8;
			goto utos;

		case 'p':
			algnr = sizeof(long) * 2;
			prefill = '0';
		case 'X':
			digsel = "01234567890ABCDEF";
		case 'x':
			base = 16;
		case 'u':
			x = get_lon_arg(&ap, lon);
		utos:
			if (sharp) {
				putch('0');
				if (c != 'o')
					putch(c);
			}
			len = geusslen(x, base);
			if (algnr && !rightalg) {
				gputtimes(putch, algnr - len, prefill);
			}
			_geutoa(putch, digsel, x, base, len);
			if (algnr && rightalg) {
				gputtimes(putch, algnr - len, prefill);
			} // TODO: consider another algl??
			break;
		}
	}
break_while:
	return 0;
}


int gprintf
	( void (*putch)(char)
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vgprintf(putch, fmt, ap);
	va_end(ap);
	return res;
}
