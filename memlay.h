#pragma once


#define PSM_BEG  0x0180000UL
#define DMA_END  0x1000000UL
#define PSM_END  (DMA_END - PGSIZE)
#define SLOB_BEG (DMA_END - 0x20)
