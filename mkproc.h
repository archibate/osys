#pragma once


extern
void __attribute__((noreturn)) process_exit(int val);

extern
PCB *create_process
	( int (*proc)()
	, void *arg
	);
