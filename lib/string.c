void strcpy(char *dst, const char *src)
{
	while ((*dst++ = *src++));
}

void strncpy(char *dst, const char *src, unsigned long n)
{
	while (n-- && (*dst++ = *src++));
}

unsigned long strlen(const char *src)
{
	unsigned long res = 0;
	while (*src++)
		res++;
	return res;
}

unsigned long strnlen(const char *src, unsigned long n)
{
	unsigned long res = 0;
	while (n-- && *src++)
		res++;
	return res;
}

int strcmp(const char *dst, const char *src)
{
	int res = 0;
	while (*dst && (res = *dst++ - *src++));
	return res;
}

int strncmp(const char *dst, const char *src, unsigned long n)
{
	int res = 0;
	while (n-- && *dst && (res = *dst++ - *src++));
	return res;
}
