#include <print.h>
#include <memlay.h>
#include <page.h>
#include <mmu.h>


#define DMA_PAGES (DMA_END >> PGSHIFT)
#define DMA_PGTS ((DMA_PAGES + PTSIZE - 1) >> PTSHIFT)
#define PGALIGNED __attribute__((aligned(PGSIZE)))
static unsigned long dma_ptes[PTSIZE * DMA_PGTS] PGALIGNED;
static unsigned long dma_pd[PTSIZE] PGALIGNED;


void init_dma
(void)
{
	unsigned long addr;
	unsigned i;

	for	( addr = 0
		, i    = 0
		; addr < DMA_END
		; addr += PGSIZE
		, i++
		)
	{
		dma_ptes[i] = addr | PG_P | PG_W;
	}

	for	( i = 0
		; i < DMA_PGTS
		; i++
		)
	{
		addr = (unsigned long) (&dma_ptes[i * PTSIZE]);
		dma_pd[i] = addr | PG_P | PG_W;
	}

	mmu_set_pd(dma_pd);

	asm volatile (
		"movl %%cr0, %%eax\n"
		"orl $0x80000000, %%eax\n"
		"movl %%eax, %%cr0\n"
		::);
}
