// vim: set ts=4 sts=4 sw=4
#include <struct.h>
#include <list.h>
#include <panic.h>
#include <memlay.h>
#include <page.h>
#include <kmalloc.h> // impelementation

#ifndef TESTING
#define tprintf(...)
#else
#include <print.h>
#define tprintf(...) printf(__VA_ARGS__)

static
void __attribute__((unused)) print_slob_status(void);

void test_slob
(void)
{
	print_slob_status();
	void *p1 = kmalloc(10);
	void *p2 = kmalloc(6);
	kfree(p1);
	void *p3 = kmalloc(45);
	kfree(p2);
	void *p4 = kmalloc(1241);
	print_slob_status();
	kfree(p3);
	void *p5 = kmalloc(1240);
	kfree(p4);
	kfree(p5);
	printf("!!survive\n");
	print_slob_status();
}
#endif

STRUCT(HNODE) {
	LIST list;
	unsigned long allocated;
};

// reference: http://wiki.0xffffff.org/posts/hurlex-11.html
static
LIST *slob_head;

void init_slob(void)
{
	slob_head = LI((HNODE*)SLOB_BEG);
	NODEOF(HNODE, slob_head)->allocated = 0;
	slob_head->next = 0;
}

static void grow_mapping_to(unsigned long adr)
{
}

static void shrink_mapping_to(unsigned long adr)
{
}

void print_slob_status(void)
{
	printf("slob status %p:\n", slob_head);
	list_foreach_(le, slob_head) {
		HNODE *curr = NODEOF(HNODE, le);
		unsigned long len = NEXT(curr) ? (char*)NEXT(curr) - (char*)curr : 0;
		printf("%c:%p:%d\n", curr->allocated ? 'M' : '-', curr, len);
	}
}

void *kmalloc(unsigned long len)
{
	len = (len + sizeof(int) - 1) & (~0UL ^ (sizeof(int) - 1));
	len += sizeof(HNODE);
	printf("!!kmalloc(%d)\n", len);

	HNODE *curr = NODEOF(HNODE, slob_head);

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
					list_link3(LI(curr), LI(rest), LI(curr)->next);
				}
				tprintf("return %p\n", curr + 1);
				return curr + 1;
			}
		}
		curr = NEXT(curr);
	}

	HNODE *next = (HNODE*)((char*)curr + len);
	tprintf("grow to %p\n", next + 1);
	grow_mapping_to((unsigned long) (next + 1));
	list_link(LI(curr), LI(next));
	LI(next)->next = 0;
	next->allocated = 0;
	curr->allocated = len;
	tprintf("return %p:%d\n", curr, len);
	return curr + 1;
}

void kfree(void *p)
{
	HNODE *curr = (HNODE*)p - 1;
	tprintf("free %p:%d\n", curr, curr->allocated);

	if (LI(curr)->next && !NEXT(curr)->allocated) {
		list_remove_nextof_s3(LI(curr));
	}

	if (LI(curr)->prev && !PREV(curr)->allocated) {
		list_remove_s2(LI(curr));
	}

	if (!LI(curr)->next) {
		LI(curr)->prev->next = 0;
		tprintf("shrink to %p\n", PREV(curr) + 1);
		shrink_mapping_to((unsigned long) (PREV(curr) + 1));
	}

	curr->allocated = 0;
}
