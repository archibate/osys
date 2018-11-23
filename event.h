#pragma once


#include <struct.h>

typedef unsigned int eve_val_t;

STRUCT(EVENT)
{
	struct TCB *waiting_head;
	unsigned int trig_count;
};

static inline
void event_init(EVENT *event)
{
	event->waiting_head = 0;
	event->trig_count = 0;
}

void wait_on_ex(EVENT *event, struct TCB *next);
void trig_up(EVENT *event);

#include <sched.h>

#define wait_on(event) wait_on_ex(event, current->next)

#include <tcb.h>
