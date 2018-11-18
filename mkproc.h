#pragma once


#include <pcb.h>

extern
PCB *create_process
	( int (*proc)()
	, void *arg
	);
