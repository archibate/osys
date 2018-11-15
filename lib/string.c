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
	while (!(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

int strncmp(const char *dst, const char *src, unsigned long n)
{
	int res = 0;
	while (n-- && !(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}
