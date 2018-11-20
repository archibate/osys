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

void putfont
	( unsigned char *p
	, int stride
	, unsigned short col
	, char c
	)
{
	const unsigned char *f = hankaku + 16 * (unsigned char) c;

	for (int j = 0; j < 16; j++) {
		unsigned char w = f[j];
		for (int i = 0; i < 8; i++) {
			if (w & 1)
				p[i] = col & 0xff;
			else
				p[i] = col >> 8;
			w >>= 1;
		}
		p += stride;
	}
}

void putstr
	( unsigned char *p
	, int stride
	, unsigned char col
	, const char *str
	)
{
	while (*str) {
		putfont(p, stride, col, *str++);
		p += 8;
	}
}

void putcursor
	( unsigned char *p
	, int stride
	, unsigned short col
	, int beg
	, int end
	)
{
	for (int j = 0; j < 16; j++) {
		char c = (beg <= j && j < end) ? (col & 0xff) : (col >> 8);
		memset(p, c, 8);
		p += stride;
	}
}

void putblack
	( unsigned char *p
	, int stride
	, unsigned short col
	)
{
	char c = col >> 8;
	for (int j = 0; j < 16; j++) {
		memset(p, c, 8);
		p += stride;
	}
}

static VIDEO_INFO video;
#define AT(x, y) (video.buf + video.xsiz * (y) * 16 + (x) * 8), video.xsiz

static int x, y;
static unsigned short col = 0x000a;

void vmon_putc(char c)
{
	putchar(c);

	if (c == '\n') {
		putblack(AT(x, y), col);
		y++;
		x = 0;
	} else {
		putfont(AT(x, y), col, c);
		x++;
	}
}

void vmon_recursor(void)
{
	fflush(stdout);

	putcursor(AT(x, y), col, 13, 16);
}

int main(void)
{
	void *winbuf = (void *) 0x80000000;
	int i, vsize;
	FILE *f;
	char c;

	i = open("/dev/vinfo", OPEN_RD);
	if (i < 0)
		error(i, "open(/dev/vinfo)");
	vsize = read(i, &video, sizeof(video));
	close(i);

	i = open("/dev/vram", OPEN_RD | OPEN_WR);
	if (i < 0)
		error(i, "open(/dev/vram)");
	vsize = tellsize(i);
	mmap(i, winbuf, vsize, MMAP_WR);
	close(i);
	video.buf = winbuf;

	i = fopen_s(&f, "/dev/vmon0", "r");
	if (!f)
		error(i, "fopen(/dev/vmon0)");

	while (EOF != (c = fgetc_ex(f, vmon_recursor)))
		vmon_putc(c);

	fclose(f);

	return 0;
}
