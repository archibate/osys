#include <kern/sysapi.h>
#include <fsdefs.h>
#include <malloc.h>
#include <vinfo.h>

struct DLL_STRPICENV {	/* 64KB */
	int work[64 * 1024 / 4];
};

struct RGB {
	unsigned char b, g, r, t;
};

/* bmp.nasm */
int info_BMP(struct DLL_STRPICENV *env, int *info, int size, char *fp);
int decode0_BMP(struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip);

/* jpeg.c */
int info_JPEG(struct DLL_STRPICENV *env, int *info, int size, char *fp);
int decode0_JPEG(struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip);

unsigned char rgb2pal(int r, int g, int b, int x, int y);
void error(char *s);
int init_heap(void);

int gview_main(const char *path)
{
	int unused = init_heap();
	struct VIDEO_INFO video;
	struct DLL_STRPICENV *env = malloc(sizeof(struct DLL_STRPICENV));
	char *filebuf = malloc(512 * 1024), *p;
	char *winbuf = (void *) 0x80000000;//video.buf;//[1040 * 805];
	int i, j, fsize, xsize, info[8];
	struct RGB *picbuf = malloc(1024 * 768 * sizeof(struct RGB)), *q;

	/* �t�@�C���ǂݍ��� */
	i = open(path, OPEN_RD); if (i < 0) { error("file not found.\n"); return 1; }
	fsize = tellsize(i);
	if (fsize > 512 * 1024) {
		error("file too large.\n");
		return 2;
	}
	read(i, filebuf, fsize);
	close(i);

	i = open("/dev/vinfo", OPEN_RD); if (i < 0) { error("can't open vinfo.\n"); return 3; }
	read(i, &video, sizeof(VIDEO_INFO));
	close(i);

	i = open("/dev/vram", OPEN_WR); if (i < 0) { error("can't open vram.\n"); return 3; }
	xsize = tellsize(i);
	mmap(i, winbuf, xsize, MMAP_WR);
	close(i);

	/* �t�@�C���^�C�v�`�F�b�N */
	if (info_BMP(env, info, fsize, filebuf) == 0) {
		/* BMP�ł͂Ȃ����� */
		if (info_JPEG(env, info, fsize, filebuf) == 0) {
			/* JPEG�ł��Ȃ����� */
			error("file type unknown.\n");
			return 4;
		}
	}
	/* �ǂ��炩��info�֐�����������ƁA�ȉ��̏��info�ɓ����Ă��� */
	/*	info[0] : �t�@�C���^�C�v (1:BMP, 2:JPEG) */
	/*	info[1] : �J���[��� */
	/*	info[2] : xsize */
	/*	info[3] : ysize */

	if (info[2] > 1024 || info[3] > 768) {
		error("picture too large.\n");
		return 5;
	}

	/* �E�B���h�E�̏��� */
	xsize = info[2] + 16;
	if (xsize < 136) {
		xsize = 136;
	}
	//win = api_openwin(winbuf, xsize, info[3] + 37, -1, "gview");

	/* �t�@�C�����e���摜�f�[�^�ɕϊ� */
	if (info[0] == 1) {
		i = decode0_BMP (env, fsize, filebuf, 4, (char *) picbuf, 0);
	} else {
		i = decode0_JPEG(env, fsize, filebuf, 4, (char *) picbuf, 0);
	}
	free(filebuf);
	free(env);
	/* b_type = 4 �́A struct RGB �`�����Ӗ����� */
	/* skip��0�ɂ��Ă����΂悢 */
	if (i != 0) {
		error("decode error.\n");
		return 6;
	}

	/* �\�� */
	xsize = video.xsiz;
	for (i = 0; i < info[3]; i++) {
		p = winbuf + (i + 29) * xsize + (xsize - info[2]) / 2;
		q = picbuf + i * info[2];
		for (j = 0; j < info[2]; j++) {
			p[j] = rgb2pal(q[j].r, q[j].g, q[j].b, j, i);
		}
	}
	free(picbuf);
	//api_refreshwin(win, (xsize - info[2]) / 2, 29, (xsize - info[2]) / 2 + info[2], 29 + info[3]);

	/* �I���҂� */
	/*for (;;) {
		i = api_getkey(1);
		if (i == 'Q' || i == 'q') {
			api_end();
		}
	}*/
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
