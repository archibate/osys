#include "wcsing.h"
#include "wchar.h"

void wcscpy(wchar_t *dst, const wchar_t *src)
{
	while ((*dst++ = *src++));
}

void wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n)
{
	while (n-- && (*dst++ = *src++));
}

void wcscat(wchar_t *dst, const wchar_t *src)
{
	wcscpy(dst + wcslen(dst), src);
}

void wcsncat(wchar_t *dst, const wchar_t *src, unsigned long n)
{
	wcsncpy(dst + wcslen(dst), src, n);
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
	while (!(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n)
{
	int res = 0;
	while (n-- && !(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

int wcsfind(const wchar_t *src, wchar_t ch)
{
	int i = 0;
	while (*src) {
		if (*src == ch)
			return i;
		i++, src++;
	}
	return -1;
}

int wcsfindl(const wchar_t *src, wchar_t ch)
{
	int i = 0;
	while (*src) {
		if (*src == ch)
			return i;
		i++, src++;
	}
	return i;
}

int wcsnfind(const wchar_t *src, wchar_t ch, unsigned long n)
{
	int i = 0;
	while (n-- && *src) {
		if (*src == ch)
			return i;
		i++, src++;
	}
	return -1;
}

wchar_t *wcsskipin(const wchar_t *dst, const wchar_t *chrs)
{
	while (wcsfind(chrs, *dst) != -1)
		dst++;
	return (wchar_t *) dst;
}

int wcsfindin(const wchar_t *src, const wchar_t *chrs)
{
	int i = 0;
	while (*src) {
		if (wcsfind(chrs, *src) != -1)
			return i;
		i++, src++;
	}
	return -1;
}

int wcsfindinl(const wchar_t *src, const wchar_t *chrs)
{
	int i = 0;
	while (*src) {
		if (wcsfind(chrs, *src) != -1)
			return i;
		i++, src++;
	}
	return i;
}

int wcschop(wchar_t *dst, const wchar_t *chrs)
{
	int i = 0;
	unsigned long len = wcslen(dst) - 1;
	while (len > 0 && wcsfind(chrs, dst[len]) != -1) {
		dst[len--] = 0;
		i++;
	}
	return 0;
}

wchar_t *wcstrim(wchar_t *dst, const wchar_t *chrs)
{
	dst = wcsskipin(dst, chrs);
	wcschop(dst, chrs);
	return dst;
}
