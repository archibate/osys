#ifndef LIST
#include <struct.h>
STRUCT(LIST)
{
	LIST *next, *prev;
};
#endif
#ifndef LMETHNAME
#define LMETHNAME list
#endif


#if 0
#include <stddef.h> // offsetof
#define NODEOF(type, lnode) ((type*)((char*)(lnode)-(long)offsetof(type,list)))
#define LI(tnode) (&((tnode)->list))
#define NEXT(tnode) NODEOF(typeof(*(tnode)), (tnode)->list.next)
#define PREV(tnode) NODEOF(typeof(*(tnode)), (tnode)->list.prev)
#endif


#undef __$$LCONCAT$
#undef _$$LCONCAT$
#undef _$LMETH
#define __$$LCONCAT$(x, y) x##_##y
#define _$$LCONCAT$(x, y) __$$LCONCAT$(x, y)
#define _$LMETH(x) _$$LCONCAT$(LMETHNAME, x)


static inline
void _$LMETH(link)
	( LIST *prev
	, LIST *next
	)
{
	prev->next = next;
	next->prev = prev;
}


static inline
void _$LMETH(link_s1)
	( LIST *prev
	, LIST *next
	)
{
	if (prev)
		prev->next = next;
	next->prev = prev;
}


static inline
void _$LMETH(link_s2)
	( LIST *prev
	, LIST *next
	)
{
	prev->next = next;
	if (next)
		next->prev = prev;
}


static inline
void _$LMETH(link_s12)
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
void _$LMETH(link3)
	( LIST *prev
	, LIST *node
	, LIST *next
	)
{
	_$LMETH(link)(prev, node);
	_$LMETH(link)(node, next);
}


static inline
void _$LMETH(remove)
	( LIST *node
	)
{
	_$LMETH(link)(node->prev, node->next);
}


static inline
void _$LMETH(remove_ch)
	( LIST **phead
	)
{
	if ((*phead)->next != (*phead))
		_$LMETH(remove)(*phead);
	else
		*phead = 0;
}


static inline
void _$LMETH(remove_s2)
	( LIST *node
	)
{
	_$LMETH(link_s2)(node->prev, node->next);
}


static inline
void _$LMETH(remove_nextof_s3)
	( LIST *node
	)
{
	_$LMETH(link_s2)(node, node->next->next);
}


static inline
void _$LMETH(replace)
	( LIST *node
	, LIST *new_node
	)
{
	_$LMETH(link3)(node->prev, new_node, node->next);
}


static inline
void _$LMETH(lookfit)
	( LIST *node
	, LIST *curr
	)
{
	node->next = curr->next;
	node->prev = curr->prev;
}


static inline
void _$LMETH(insert_before)
	( LIST *new_node
	, LIST *node
	)
{
	_$LMETH(link3)(node->prev, new_node, node);
}


static inline
void _$LMETH(insert_after)
	( LIST *new_node
	, LIST *node
	)
{
	_$LMETH(link3)(node, new_node, node->next);
}


static inline
void _$LMETH(insert_before_ch)
	( LIST *new_node
	, LIST **pnode
	)
{
	if (*pnode)
		_$LMETH(insert_before)(new_node, *pnode);
	else {
		new_node->next = new_node->prev = new_node;
		*pnode = new_node;
	}
}


static inline
void _$LMETH(insert_after_ch)
	( LIST *new_node
	, LIST **pnode
	)
{
	if (*pnode)
		_$LMETH(insert_after)(new_node, *pnode);
	else {
		new_node->next = new_node->prev = new_node;
		*pnode = new_node;
	}
}


static inline
LIST *_$LMETH(shift_forward)
	( LIST **phead
	)
{
	LIST *head = *phead;
	_$LMETH(remove)(head);
	*phead = head->next;
	return head;
}


static inline
LIST *_$LMETH(shift_backward)
	( LIST **phead
	)
{
	LIST *head = *phead;
	_$LMETH(remove)(head);
	*phead = head->next;
	return head;
}


static inline
LIST *_$LMETH(shift_forward_ch)
	( LIST **phead
	)
{
	LIST *head = *phead;
	if (head->next != head)
		_$LMETH(shift_forward)(phead);
	else
		*phead = 0;
	return head;
}


static inline
LIST *_$LMETH(shift_backward_ch)
	( LIST **phead
	)
{
	LIST *head = *phead;
	if (head->next != head)
		_$LMETH(shift_backward)(phead);
	else
		*phead = 0;
	return head;
}


#ifndef list_for
#define list_for(le, head) for (le = (head); le; le = le->next)
#endif
#ifndef list_foreach
#define list_foreach(le, head) for (typeof(head) le = (head); le; le = le->next)
#endif


#undef LIST
#undef LMETHNAME
