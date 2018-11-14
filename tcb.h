#pragma once


#include <pcb.h>
#include <struct.h>

STRUCT(TCB)
{
	TCB *next, *prev;

	PCB *pcb;
};
#define LIST TCB
#define LMETHNAME tcb
#include <list.h>
