#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <vinfo.h>

static VIDEO_INFO video;
#define AT(x, y) (video.buf + video.xsiz * y + x)

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

	i = fopen_s(&f, "/tmp/.xwm-pipe", "ra"); // TODO
	if (!f)
		error(i, "fopen(/tmp/.xwm-pipe)");

	while (1) {
		s = fgetrdbuf(f);
		vmon_puts(s);
		vmon_recursor();
	}

	fclose(f);

	return 0;
}
