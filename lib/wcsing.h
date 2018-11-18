#pragma once

#include <wchar.h>


void wcscpy(wchar_t *dst, const wchar_t *src);
void wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n);
void wcscat(wchar_t *dst, const wchar_t *src);
void wcsncat(wchar_t *dst, const wchar_t *src, unsigned long n);
unsigned long wcslen(const wchar_t *src);
unsigned long wcsnlen(const wchar_t *src, unsigned long n);
int wcscmp(const wchar_t *dst, const wchar_t *src);
int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n);
int wcsfind(const wchar_t *src, wchar_t ch);
int wcsfindin(const wchar_t *src, const wchar_t *chrs);
wchar_t *wcsskip(const wchar_t *dst, const wchar_t *chrs);
int wcschop(wchar_t *dst, const wchar_t *chrs);
wchar_t *wcstrim(wchar_t *dst, const wchar_t *chrs);
