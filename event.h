#pragma once


#include <struct.h>
#include <tcb.h>

STRUCT(EVENT)
{
	TCB *waiting_head;
	//TCB *feeder;
};

static inline
void event_init
	( EVENT *event
	)
{
	event->waiting_head = 0;
	//event->feeder = 0;
}

extern
void wait_on
	( EVENT *event
	);

extern
void trig_up
	( EVENT *event
	);
