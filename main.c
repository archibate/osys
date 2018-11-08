#define INIT(x) extern void init_##x(void); init_##x();
int main
(void)
{
	INIT(idt);
	INIT(idtfill);

	INIT(pic);
	INIT(kbc);
	INIT(mouse);

	INIT(game);
}


// GAME ZONE

struct VIDEO_INFO
{
	unsigned char *buf;
	unsigned short xsiz;
	unsigned short ysiz;
	unsigned short vmode;
	unsigned char bpp;
	unsigned char mmodel;
} __attribute__((packed));


void init_game
(void)
{
	struct VIDEO_INFO *video = (struct VIDEO_INFO *) 0x7c00;

	for (int i = 0; i < video->xsiz * video->ysiz; i++)
		video->buf[i] = (i % video->xsiz) % 0x10;
}
