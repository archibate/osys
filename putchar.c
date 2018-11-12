#include "serial.h" // serial_putc


int putchar // to override for weaken one in libc.c
(int c)
{
	serial_putc(c);
	return c;
}
