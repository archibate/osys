#pragma once


#define PGSHIFT 12
#define PTSHIFT 10
#define PGSIZE  0x00001000UL
#define PGATTR  0x00000fffUL
#define PGMASK  0xfffff000UL
#define PTSIZE  0x00000400UL
#define PTMASK  0xfffffc00UL
#define PTATTR  0x000003ffUL

#define PG_P (1UL<<0)
#define PG_W (1UL<<1)
#define PG_U (1UL<<2)
