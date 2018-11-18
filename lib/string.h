#pragma once


void strcpy(char *dst, const char *src);
void strncpy(char *dst, const char *src, unsigned long n);
void strcat(char *dst, const char *src);
void strncat(char *dst, const char *src, unsigned long n);
unsigned long strlen(const char *src);
unsigned long strnlen(const char *src, unsigned long n);
int strcmp(const char *dst, const char *src);
int strncmp(const char *dst, const char *src, unsigned long n);
int strfind(const char *src, char ch);
int strfindin(const char *src, const char *chrs);
char *strskip(const char *dst, const char *chrs);
int strchop(char *dst, const char *chrs);
char *strtrim(char *dst, const char *chrs);
