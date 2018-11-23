#pragma once


#include <pcb.h>

extern
void switch_from_to
	( PCB *prev
	, PCB *next
	);
extern
void switch_pgd
	( unsigned long *pgd
	);
extern
void setup_pgd
	( unsigned long *pgd
	);
