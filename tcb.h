#pragma once


#include <pcb.h>
#include <tcbdefs.h>
#include <struct.h>
#include <event.h>

STRUCT(TCB)
{
	TCB *next, *prev;

	PCB *pcb;

	int exit_res;

	EVENT events[EVE_MAX];
};
#define LIST TCB
#define LMETHNAME tcb
#include <list.h>
