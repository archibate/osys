#pragma once


#include "struct.h"


STRUCT(LIST)
{
	LIST *next, *prev;
};

#define SLNODE(x) ((LIST) { .next = (x), .prev = (x), })
#define STNODE(tnode) SLNODE(LNOD(tnode))


#include "stddef.h" // offsetof

#define NODE(type, lnode) ((type*)((char*)(lnode)-(long)offsetof(type,list)))
#define LNOD(tnode) (&((tnode)->list))
#define NEXT(tnode) NODE(typeof(*(tnode)), (tnode)->list.next)
#define PREV(tnode) NODE(typeof(*(tnode)), (tnode)->list.prev)


static inline
void list_link
	( LIST *prev
	, LIST *next
	)
{
	prev->next = next;
	next->prev = prev;
}


static inline
void list_remove
	( LIST *node
	)
{
	list_link(node->prev, node->next);
}
