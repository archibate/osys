#pragma once


#include <struct.h>

typedef unsigned int eve_val_t;

STRUCT(EVENT)
{
	struct TCB *waiting_head;
	eve_val_t e_val;
};

static inline
void event_init(EVENT *event)
{
	event->waiting_head = 0;
}

eve_val_t wait_on(EVENT *event);
void trig_up(EVENT *event, eve_val_t val);

STRUCT(IEVENT)
{
	EVENT eve;
	eve_val_t val;
};
