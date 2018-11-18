#pragma once


#define PGSHIFT 12
#define PTSHIFT 10
#define PGSIZE  0x00001000UL
#define PGATTR  0x00000fffUL
#define PGMASK  0xfffff000UL
#define PTSIZE  0x00000400UL
#define PTMASK  0xfffffc00UL
#define PTATTR  0x000003ffUL

#define PG_P   (1UL<<0)
#define PG_W   (1UL<<1)
#define PG_U   (1UL<<2)
#define PG_PWT (1UL<<3)
#define PG_PCD (1UL<<4)
#define PG_A   (1UL<<5)
#define PG_D   (1UL<<6)  // pte only
#define PG_PS  (1UL<<7)  // pde only
#define PG_PAT (1UL<<8)  // pte only
#define PG_G   (1UL<<8)
#define PG_PSM (1UL<<9)
#define PG_PDM (1UL<<10) // pde only
