#pragma once


#include <struct.h>
#include <tcb.h>

STRUCT(EVENT)
{
	TCB *waiting_head;
};


extern
void wait_on
	( EVENT *event
	);

extern
void trig_up
	( EVENT *event
	);
