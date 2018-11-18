#pragma once


#define PSM_BEG  0x180000UL
#define DMA_END  0x800000UL
#define PSM_END  (DMA_END - PGSIZE)
#define SLOB_BEG (DMA_END - 0x20)
