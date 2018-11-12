#pragma once


#include <struct.h>


STRUCT(LIST)
{
	LIST *next, *prev;
};

STRUCT(LIST_HEAD)
{
	LIST *head;
};

#define SLIE(x) ((LIST) { .next = (x), .prev = (x), })
#define STNODE(tnode) SLIE(LI(tnode))


#include <stddef.h> // offsetof

#define NODEOF(type, lnode) ((type*)((char*)(lnode)-(long)offsetof(type,list)))
#define LI(tnode) (&((tnode)->list))
#define NEXT(tnode) NODEOF(typeof(*(tnode)), (tnode)->list.next)
#define PREV(tnode) NODEOF(typeof(*(tnode)), (tnode)->list.prev)


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
void list_link3
	( LIST *prev
	, LIST *node
	, LIST *next
	)
{
	list_link(prev, node);
	list_link(node, next);
}


static inline
void list_remove
	( LIST *node
	)
{
	list_link(node->prev, node->next);
}


static inline
void list_replace
	( LIST *node
	, LIST *new_node
	)
{
	list_link3(node->prev, new_node, node->next);
}
