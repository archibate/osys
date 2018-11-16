#pragma once


static inline
unsigned int rdtsc(void)
{
	unsigned int res;
	asm volatile ("rdtsc" : "=a" (res) :: "edx");
	return res;
}
