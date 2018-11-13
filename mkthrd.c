#include <tcb.h>
#include <sched.h>
#include <kmalloc.h>


TCB *create_thread
	( PCB *pcb
	)
{
	TCB *tcb = kmalloc(sizeof(TCB));
	//bzero(tcb, sizeof(TCB));
	tcb->pcb = pcb;
	add_task(tcb);
	return tcb;
}
