#pragma once

#include <stdarg.h>


// Main function to format and print a string.
void printfmt(void (*putch)(int, void*), void *putdat, const char *fmt, ...);

void
vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list ap);

int
vsnprintf(char *buf, int n, const char *fmt, va_list ap);

int
snprintf(char *buf, int n, const char *fmt, ...);
