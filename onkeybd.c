#include <onkeybd.h>
#include <print.h>


void on_keyboard_event
	( unsigned char scancode
	)
{
	printf("keyboard %#02x\n", scancode);
}
