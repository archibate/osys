#include <onmouse.h>
#include <print.h>


void on_mouse_event
	( unsigned char mbtn
	, int mdx
	, int mdy
	)
{
	printf("mouse %d %d %d\n", mbtn, mdx, mdy);
}
