#include <unistd.h>
#include <string.h>

int main(void)
{
	const char s[] = "Hello, World\n";
	write(1, s, strlen(s));
	return 0;
}
