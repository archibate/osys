#pragma once


#include <pcb.h>
#include <struct.h>
#include <event.h>

STRUCT(TCB)
{
	TCB *next, *prev;

	PCB *pcb;

	EVENT eve_exited;
};
#define LIST TCB
#define LMETHNAME tcb
#include <list.h>
