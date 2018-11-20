#include <kern/sysapi.h>
#include <fsdefs.h>
#include <vinfo.h>

void error(char *s);
unsigned char rgb2pal(int r, int g, int b, int x, int y);

int main(void)
{
	struct VIDEO_INFO video;
	char *p, *winbuf = (void *) 0x80000000;
	int i, j, xsize;
	int r, g, b, s = 1;

	i = open("/dev/vinfo", OPEN_RD);
	if (i < 0) { error("can't open vinfo.\n"); return 3; }
	read(i, &video, sizeof(VIDEO_INFO));
	close(i);

	i = open("/dev/vram", OPEN_WR);
	if (i < 0) { error("can't open vram.\n"); return 3; }
	xsize = tellsize(i);
	mmap(i, winbuf, xsize, MMAP_WR);
	close(i);

	p = winbuf;
	for (i = 0; i < video.ysiz; i++) {
		for (j = 0; j < video.xsiz; j++) {
			r = g = (i * 255) / video.ysiz;
			s = s * 591245 + 125;
			b = (s / 255) % 128;
			*p++ = rgb2pal(r, g, b, j, i);
		}
	}

	return 0;
}

unsigned char rgb2pal(int r, int g, int b, int x, int y)
{
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1; /* ��������� */
	y &= 1;
	i = table[x + y * 2];	/* ���ԐF����邽�߂̒萔 */
	r = (r * 21) / 256;	/* ����� 0�`20 �ɂȂ� */
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;	/* ����� 0�`5 �ɂȂ� */
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}

void error(char *s)
{
	//printf(s);
}
