#define INIT(x, ...) extern void init_##x(); init_##x(__VA_ARGS__);
#ifdef TESTING
#define TEST(x, ...) extern void test_##x(); test_##x(__VA_ARGS__);
#else
#define TEST(x, ...)
#endif
int main
(void)
{
	INIT(idt);
	INIT(pic);
	INIT(kbc);
	INIT(serial);
	INIT(keyboard);
	INIT(mouse);
	INIT(timer, 100);
	INIT(psm);
	INIT(game);
	INIT(dma);
	INIT(slob);
	TEST(slob);
	INIT(sched);
	TEST(sched);
	asm volatile ("sti");

	for (;;)
		asm volatile ("hlt");
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
	struct VIDEO_INFO *video = (struct VIDEO_INFO *) 0x7b00;

	for (int i = 0; i < video->xsiz * video->ysiz; i++)
		video->buf[i] = (i % video->xsiz) % 0x10;
}
