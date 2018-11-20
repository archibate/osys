#include <txtdev.h>
#include <string.h>


void init_welcome(void)
{
	const char welcome[] = "\n\nOSYS kernel v0.1 (C) archibate 2018\n\n";
	make_txtinfo_dev("welcome", welcome, strlen(welcome));
}
