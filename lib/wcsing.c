#include <wchar.h>

void wcscpy(wchar_t *dst, const wchar_t *src)
{
	while ((*dst++ = *src++));
}

void wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n)
{
	while (n-- && (*dst++ = *src++));
}

unsigned long wcslen(const wchar_t *src)
{
	unsigned long res = 0;
	while (*src++)
		res++;
	return res;
}

unsigned long wcsnlen(const wchar_t *src, unsigned long n)
{
	unsigned long res = 0;
	while (n-- && *src++)
		res++;
	return res;
}

int wcscmp(const wchar_t *dst, const wchar_t *src)
{
	int res = 0;
	while (*dst && !(res = *dst++ - *src++));
	return res;
}

int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n)
{
	int res = 0;
	while (n-- && *dst && (res = *dst++ - *src++));
	return res;
}
