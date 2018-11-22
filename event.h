#pragma once


#include <struct.h>

STRUCT(EVENT)
{
	struct TCB *waiting_head;
	//struct TCB *feeder;
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
