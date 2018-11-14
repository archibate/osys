#pragma once

#include <wchar.h>


void wcscpy(wchar_t *dst, const wchar_t *src);
void wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n);
unsigned long wcslen(const wchar_t *src);
unsigned long wcsnlen(const wchar_t *src, unsigned long n);
int wcscmp(const wchar_t *dst, const wchar_t *src);
int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n);
