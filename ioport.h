#pragma once



static inline
unsigned char io_inb
	( unsigned short port
	)
{
	unsigned char data;
	asm volatile ("inb %%dx, %%al\n" : "=a" (data) : "d" (port));
	return data;
}

static inline
void io_outb
	( unsigned short port
	, unsigned char data
	)
{
	asm volatile ("outb %%al, %%dx\n" :: "a" (data), "d" (port));
}
