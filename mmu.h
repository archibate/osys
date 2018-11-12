#pragma once


#define mmu_set_pd(pgd) \
	asm volatile ("movl %0, %%cr3" :: "r" (pgd))


#define mmu_get_pd(pgd) ({ \
	unsigned long $$$pgd; \
	asm volatile ("movl %%cr3, %0" : "=r" ($$$pgd)) \
	$$$pgd; \
	})

#define mmu_flush_tlb() \
	asm volatile ("movl %%cr3, %%eax; movl %%eax, %%cr3")

#define mmu_invalidate(va) \
	asm volatile ("invlpg %0" :: "m" ((volatile void *)(va)))
