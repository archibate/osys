#include "onkeybd.h"
#include "print.h"


void on_keyboard_event
	( unsigned char scancode
	)
{
	printf("keyboard 0x%x\n", scancode);
}
