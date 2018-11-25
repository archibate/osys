// vim: ts=4 sts=4 sw=4
// reference: http://wiki.0xffffff.org/posts/hurlex-11.html
#include <malloc.h> // impelementation
#include <kern/page.h> // PGSIZE
#include <sizet.h> // size_t
typedef size_t addr_t;

#include <unistd.h> // setbrk
#include <assert.h> // assert

#include <umemlay.h> // USER_STACK_BEG
#define SLOB_BEG USER_STACK_BEG

#include <struct.h> // STRUCT()
STRUCT(HNODE) {
	HNODE *next, *prev;
	size_t allocated;
};
#define LIST HNODE
#include <list.h> // define the list_* methods for HNODE

static
HNODE *heap_head;

static
void set_break(void *p)
{
	static void *curr = 0;
	p += PGSIZE;
	if (curr != p)
		setbrk(curr = p);
}

void init_heap(void)
{
	heap_head = (HNODE*)SLOB_BEG;
	set_break((void *)(PGMASK & (PGSIZE - 1 + (addr_t) heap_head)));
	heap_head->allocated = 0;
	heap_head->next = 0;
	heap_head->prev = heap_head;
}

#ifdef tprintf
void print_heap_status(void)
{
	tprintf("heap status %p:\n", heap_head);
	list_foreach(curr, heap_head) {
		size_t len = curr->next ?
				(char*)curr->next - (char*)curr : 0;
		tprintf("%c:%p:%d\n", curr->allocated ? 'M' : '-', curr, len);
	}
}
#endif

void *malloc(size_t len)
{
	len = (len + sizeof(int) - 1) & (~0UL ^ (sizeof(int) - 1));
	len += sizeof(HNODE);

	HNODE *curr;
	for (curr = heap_head; curr->next; curr = curr->next)
	{
		if (!curr->allocated) {
			ssize_t rest_len = (char*)curr->next - (char*)curr - len;
			if (rest_len > 0) {
				curr->allocated = len;
				if (rest_len > sizeof(HNODE)) {
					HNODE *rest = (HNODE*)((char*)curr + len);
					rest->allocated = 0;
					list_link3(curr, rest, curr->next);
				}
				return curr + 1;
			}
		}
	}

	HNODE *next = (HNODE*)((char*)curr + len);
	set_break(next + 1);
	list_link(curr, next);
	next->next = 0;
	next->allocated = 0;
	curr->allocated = len;
	return curr + 1;
}

void free(void *p)
{
	assert(p);

	HNODE *curr = (HNODE*)p - 1;

	if (curr->next && !curr->next->allocated) {
		list_remove_nextof_s3(curr);
	}

	if (curr->prev && !curr->prev->allocated) {
		list_remove_s2(curr);
	}

	/*if (!curr->next) {// && curr->prev && curr->prev->allocated) {
		curr->prev->next = 0;
		FIXME!
		setbrk(curr->prev + 1);
	}*/

	curr->allocated = 0;
}
