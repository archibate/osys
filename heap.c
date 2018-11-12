#include <struct.h>
#include <list.h>
#include <panic.h>
#include <memlay.h>
#include <kmalloc.h> // impelementation

#ifndef TESTING
#define tprintf(...)
#else
#include <print.h>
#define tprintf(...) printf(__VA_ARGS__)

void test_heap
(void)
{
	void *p1 = kmalloc(10);
	void *p2 = kmalloc(6);
	kfree(p1);
	void *p3 = kmalloc(45);
	kfree(p2);
	void *p4 = kmalloc(1241);
	kfree(p3);
	void *p5 = kmalloc(1240);
	kfree(p4);
	kfree(p5);
	printf("!!survive\n");
}
#endif

void __attribute__((noreturn)) oom(void)
{
	panic("Out of memory");
}


STRUCT(HNODE) {
	LIST list;
	unsigned long allocated;
};

// referenced & improved from: http://wiki.0xffffff.org/posts/hurlex-11.html
static
LIST free;

void init_heap(void)
{
	free.next = (LIST *) HEAP_BEG;
	NODEOF(HNODE, free.next)->allocated = 0;
	NODEOF(HNODE, free.next->next)->allocated = sizeof(HNODE);
	free.next->next = (LIST *) (HEAP_END - sizeof(HNODE));
	free.next->next->prev = free.next;
	free.prev = free.next->next->next = 0;
}

void *kmalloc(unsigned long len)
{
	len = (len + sizeof(long) - 1) / sizeof(long);
	len += sizeof(HNODE);

	HNODE *curr = NODEOF(HNODE, free.next);

	while (LI(curr)->next) // for the last blk is unused and { .next = NULL }
	{
		if (!curr->allocated) {
			long rest_len = (char*)NEXT(curr) - (char*)curr - len;
			tprintf("%p rest:%d\n", curr, rest_len);
			if (rest_len > 0) {
				curr->allocated = len;
				if (rest_len > sizeof(HNODE)) {
					HNODE *rest = (HNODE*)((char*)curr + len);
					rest->allocated = 0;
					tprintf("%p:%p:%p\n", curr, rest, NEXT(curr));
					list_link3(LI(curr), LI(rest), LI(NEXT(curr)));
				}
				tprintf("return %p\n", curr + 1);
				return curr + 1;
			}
		}
		curr = NEXT(curr);
	}

	oom();
}

void kfree(void *p)
{
	HNODE *curr = (HNODE*)p - 1;

	if (PREV(curr) && !NEXT(curr)->allocated) {
		list_remove(LI(NEXT(curr)));
	}

	if (PREV(curr) && !PREV(curr)->allocated) {
		list_remove(LI(PREV(curr)));
	}

	curr->allocated = 0;
}
