#pragma once


void memcpy(void *dst, const char *src, unsigned long n);
int memcmp(const void *dst, const char *src, unsigned long n);
void memset(void *dst, char val, unsigned long n);

static inline void *bzero(void *dst, unsigned long n)
{
	memset(dst, 0, n);
	return dst;
}

static inline void *bcopy(const void *src, void *dst, unsigned long n)
{
	memcpy(dst, src, n);
	return dst;
}
