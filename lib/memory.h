#pragma once


void memcpy(void *dst, const void *src, unsigned long n);
void memrcpy(void *dst, const void *src, unsigned long n);
int memcmp(const void *dst, const void *src, unsigned long n);
void memset(void *dst, char val, unsigned long n);

static inline void memmove(void *dst, const void *src, unsigned long n)
{
	if (dst > src)
		return memrcpy(dst, src, n);
	else
		return memcpy(dst, src, n);
}

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
