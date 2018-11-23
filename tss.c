#include <tss.h>
#include <stddef.h> // offsetof
#include <segdesc.h>
#include <struct.h>
#include <memlay.h>

void set_tss_seg(TSS *tss, unsigned int limit)
{
	struct {
		unsigned short limit;
		SEGDESC *gdt;
	} __attribute__((packed)) gdtr;
	asm volatile ("sgdt %0\n" :: "m" (gdtr) : "memory");

	gdtr.gdt[5] = SEG16(STS_T32A, (unsigned long) tss, limit, 0);
	gdtr.gdt[5].sd_s = 0;

	asm volatile ("lgdt %0\n" :: "m" (gdtr));

	asm volatile ("ltr %%ax\n" :: "a" (5 * 8));
}

void set_tss_data(TSS *tss, unsigned long esp, unsigned int iomap_off)
{
	tss->ts_esp0 = esp;
	tss->ts_ss0 = 0x0010;
	tss->ts_iomb = offsetof(TSS, ts_iomaps) + iomap_off;
	tss->ts_iomaps[0] = 0xff; // for now
}

void init_tss(void)
{
	TSS *tss = (TSS *) TSS0_ADDR;
	set_tss_seg(tss, sizeof(tss));
	set_tss_data(tss, IFRAME_TOP, 0);
}
