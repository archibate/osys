#pragma once


#include <tcb.h>

extern
void switch_from_to
	( TCB *prev
	, TCB *next
	);
extern
void switch_pgd
	( unsigned long *pgd
	);
extern
void setup_pgd
	( unsigned long *pgd
	);
