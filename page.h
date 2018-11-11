#pragma once


#define PGSHIFT 12
#define PTSHIFT 10
#define PGSIZE  0x00001000UL
#define PGMASK  0xfffff000UL
#define PTSIZE  0x00000400UL

#define PG_P (1UL<<0)
#define PG_W (1UL<<1)
#define PG_U (1UL<<2)
