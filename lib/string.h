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
int strfindl(const char *src, char ch);
int strnfind(const char *src, char ch, unsigned long n);
int strfindin(const char *src, const char *chrs);
int strfindlin(const char *src, const char *chrs);
char *strskipin(const char *dst, const char *chrs);
int strchop(char *dst, const char *chrs);
char *strtrim(char *dst, const char *chrs);
