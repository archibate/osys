#include <ioport.h>

void init_palette(void)
{
	int r, g, b;

	io_outb(0x3c8, 16);

	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				io_outb(0x3c9, r * 51 / 4);
				io_outb(0x3c9, g * 51 / 4);
				io_outb(0x3c9, b * 51 / 4);
			}
		}
	}
}
