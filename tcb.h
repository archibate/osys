#pragma once


#include <list.h>
#include <struct.h>
#include <pcb.h>


STRUCT(TCB)
{
	LIST list;
	PCB *pcb;
};
