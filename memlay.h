#pragma once

// for now:
#define PSM_BEG		0x00180000UL
#define PSM_END		0x00ef0000UL
#define TSS0_ADDR	0x00f20000UL
#define DMA_END		0x01000000UL
#define SLOB_BEG	(DMA_END - 0x20)

#define IFRAME_BOTT	0x0ffc0000UL
#define IFRAME_TOP	0x0ffc8000UL
#define USER_BEG	0x10000000UL
