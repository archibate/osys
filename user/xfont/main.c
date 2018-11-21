#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <vinfo.h>
#include "hankaku.h"

void error(int i, const char *s)
{
	fprintf(stderr, "%s: %m\n", s, i);
	exit(i);
}

#define COL_HILIGHT 0x00008
#define COL_UNDERLN 0x10000
#define COL_LIGHTIN 0x20000

void putfont
	( unsigned char *p
	, int stride
	, unsigned int col
	, char c
	)
{
	const unsigned char *f = hankaku + 16 * (unsigned char) c;

	for (int j = 0; j < ((col & COL_UNDERLN) ? 15 : 16); j++) {
		unsigned char w = f[j];
		for (int i = 0; i < 8; i++) {
			if (w & 1)
				p[i] = col & 0xff;
			else
				p[i] = (col >> 8) & 0xff;
			w >>= 1;
		}
		p += stride;
	}
	if (col & COL_UNDERLN) {
		memset(p, (col & 0xff), 8);
	}
}

void putcursor
	( unsigned char *p
	, int stride
	, unsigned int col
	, int beg
	, int end
	)
{
	for (int j = 0; j < 16; j++) {
		char c = (beg <= j && j < end) ?
			(col & 0xff) : ((col >> 8) & 0xff);
		memset(p, c, 8);
		p += stride;
	}
}

void putblack
	( unsigned char *p
	, int stride
	, unsigned int col
	, int width
	)
{
	char c = col >> 8;
	for (int j = 0; j < 16; j++) {
		memset(p, c, width);
		p += stride;
	}
}

static VIDEO_INFO video;
#define AT(x, y) (video.buf + video.xsiz * y + x), video.xsiz
#define ymax (video.ysiz - 16)
#define xmax (video.xsiz - 8)

static int x = 0, y = 0, old_x, old_y, cur_min = 0, cur_max = 3;
static unsigned int col = 0x000a;

void vmon_scroll_up(int h)
{
	if (h >= video.ysiz)
		h = video.ysiz;
	memcpy(video.buf, video.buf + video.xsiz * h, video.xsiz * (video.ysiz - h));
	bzero(video.buf + video.xsiz * (video.ysiz - h), video.xsiz * h);
}

void vmon_putc(char c)
{
	putchar(c);

	if (c == '\n')
	{
		putblack(AT(x, y), col, 8);
		y += 16;
		x = 0;

		if (y > ymax) {
			vmon_scroll_up(y - ymax);
			y = ymax;
		}
	}
	else
	{
		putfont(AT(x, y), col, c);
		x += 8;
	}
}

void vmon_setcolor(int num)
{
	switch (num) {
	case 0: col = 0x0007; return;
	case 1: col |= 0x0008; return;
	case 2: col |= 0x0800; return;
	case 4: col |= COL_UNDERLN; return;
	case 5: col |= COL_LIGHTIN; return;
	case 7: col ^= 0xffff; return;
	case 8: col &= 0xffff; return;
	}
	num -= 30;
	if (0 <= num && num < 8) {
		col = (col & ~0xff) | num;
		return;
	}
	num -= 10;
	if (0 <= num && num < 8) {
		col = (col & ~0xff00) | (num << 8);
		return;
	}
}

void vmon_puts(const char *s)
{
	int num, num_old;

	while (*s) {
		if (s[0] == '\033' && s[1] == '[') {
			num_old = num = 0;

			for (s += 2; *s; s++) {
				if ('0' <= *s && *s <= '9')
					num = 10 * num + *s - '0';
				else if (*s == ';')
					num_old = num;

				else {
					switch (*s) {
					case 'm':
						if (num_old)
							vmon_setcolor(num_old);
						vmon_setcolor(num);
						break;
					case 'A': y -= num * 16; if (y < 0) y = 0; break;
					case 'B': y += num * 16; if (y > ymax) y = ymax; break;
					case 'C': x += num * 8; if (x > xmax) y = xmax; break;
					case 'D': x -= num * 8; if (x < 0) x = 0; break;
					case 'J': vmon_scroll_up(num * ymax); break;
					case 'K': putblack(AT(x + 8, y), col, xmax - x + 8); break;
					case 'H': x = num; y = num_old; break;
					case 's': old_x = x; old_y = y; break;
					case 'u': x = old_x; y = old_y; break;
					case 'l': cur_min = 0; cur_max = 0; break;
					case 'h': cur_min = num_old; cur_max = (num ? num : 3); break;
					}
					break;
				}
			}
		}

		vmon_putc(*s++);
	}
}

void vmon_recursor(void)
{
	file_wr_flush(stdout);

	putcursor(AT(x, y), col, 16 - cur_max, 16 - cur_min);
}

int main(void)
{
	void *winbuf = (void *) 0x80000000;
	const char *s;
	int i, size;
	FILE *f;

	i = open("/dev/vinfo", OPEN_RD);
	if (i < 0)
		error(i, "open(/dev/vinfo)");
	size = read(i, &video, sizeof(video));
	close(i);

	i = open("/dev/vram", OPEN_RD | OPEN_WR);
	if (i < 0)
		error(i, "open(/dev/vram)");
	size = tellsize(i);
	mmap(i, winbuf, size, MMAP_WR);
	close(i);
	video.buf = winbuf;

	i = fopen_s(&f, "/dev/vmon0", "r");
	if (!f)
		error(i, "fopen(/dev/vmon0)");

	while (1) {
		s = fgetrdbuf(f);
		vmon_puts(s);
		vmon_recursor();
	}

	fclose(f);

	return 0;
}
