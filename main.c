#include <print.h>
#include <mkthrd.h>
#include <mkproc.h>
#include <pexit.h>


extern
int __attribute__((noreturn)) idle_func
(void *__attribute__((unused)) unused);

int __attribute__((noreturn)) test_func
(void *__attribute__((unused)) unused)
{
	for (;;) {
		printf("T");
		asm volatile ("sti; hlt");
	}
}

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
	printf("\nOSYS kernel v0.1 (C) archibate 2018\n\n");
	INIT(palette);
	INIT(keyboard);
	INIT(mouse);
	INIT(timer, 100);
	INIT(psm);
	INIT(dma);
	INIT(game);
	INIT(slob);
	INIT(sched);
	create_thread(create_process(idle_func, 0));
	//create_thread(create_process(test_func, 0));
	INIT(ramfs);
	INIT(devfs);
	INIT(zero);
	INIT(welcome);
	INIT(vram);
	INIT(kbd);
	INIT(mon);
	INIT(tss);
	INIT(shell);
	process_exit(0);
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


#include <map.h>
#include <panic.h>

void init_game
(void)
{
	struct VIDEO_INFO *video = (struct VIDEO_INFO *) 0x7b00;
	printf("video: mode %x at %p (%dx%d %dbpp)\n", video->vmode,
			video->buf, video->xsiz, video->ysiz,
			video->bpp);

	unsigned long base = (unsigned long) video->buf;
	assert(!(base & PGATTR));
	for (int i = 0; i < video->xsiz * video->ysiz; i += PGSIZE)
		map(base + i, (base + i) | PG_P | PG_W | PG_U | PG_G);
	mmu_flush_tlb();

	/*for (int i = 0; i < video->xsiz * video->ysiz; i++)
		video->buf[i] = (i % video->xsiz) % 0x10;*/
}
