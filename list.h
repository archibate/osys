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

#define SELF_LOOP(x) ((LIST) { .next = (x), .prev = (x), })


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
void list_link_s1
	( LIST *prev
	, LIST *next
	)
{
	if (prev)
		prev->next = next;
	next->prev = prev;
}


static inline
void list_link_s2
	( LIST *prev
	, LIST *next
	)
{
	prev->next = next;
	if (next)
		next->prev = prev;
}


static inline
void list_link_s12
	( LIST *prev
	, LIST *next
	)
{
	if (prev)
		prev->next = next;
	if (next)
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
void list_remove_s2
	( LIST *node
	)
{
	list_link_s2(node->prev, node->next);
}


static inline
void list_remove_nextof_s3
	( LIST *node
	)
{
	list_link_s2(node, node->next->next);
}


static inline
void list_replace
	( LIST *node
	, LIST *new_node
	)
{
	list_link3(node->prev, new_node, node->next);
}


static inline
void list_lookfit
	( LIST *node
	, LIST *curr
	)
{
	node->next = curr->next;
	node->prev = curr->prev;
}


static inline
void list_insert_before
	( LIST *node
	, LIST *new_node
	)
{
	list_link3(node, new_node, node->next);
}


static inline
void list_insert_after
	( LIST *node
	, LIST *new_node
	)
{
	list_link3(node->prev, new_node, node);
}


#define list_foreach(le, head) for (le = (head); le; le = le->next)
#define list_foreach_(le, head) for (typeof(head) le = (head); le; le = le->next)
