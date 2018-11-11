#pragma once


#define mmu_set_pd(pgd) \
	asm volatile ("movl %0, %%cr3" :: "r" (pgd))
