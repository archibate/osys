// vim: ts=4 sts=4 sw=4
#include <struct.h>
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
	HNODE *next, *prev;
	unsigned long allocated;
};
#define LIST HNODE
#include <list.h>

// reference: http://wiki.0xffffff.org/posts/hurlex-11.html
static
HNODE *slob_head;

void init_slob(void)
{
	slob_head = (HNODE*)SLOB_BEG;
	slob_head->allocated = 0;
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
	list_foreach(curr, slob_head) {
		unsigned long len = curr->next ?
				(char*)curr->next - (char*)curr : 0;
		printf("%c:%p:%d\n", curr->allocated ? 'M' : '-', curr, len);
	}
}

void *kmalloc(unsigned long len)
{
	len = (len + sizeof(int) - 1) & (~0UL ^ (sizeof(int) - 1));
	len += sizeof(HNODE);
	printf("kmalloc %d\n", len);

	HNODE *curr;
	for (curr = slob_head; curr->next; curr = curr->next)
	{
		if (!curr->allocated) {
			long rest_len = (char*)curr->next - (char*)curr - len;
			tprintf("%p rest:%d\n", curr, rest_len);
			if (rest_len > 0) {
				curr->allocated = len;
				if (rest_len > sizeof(HNODE)) {
					HNODE *rest = (HNODE*)((char*)curr + len);
					rest->allocated = 0;
					tprintf("%p:%p:%p\n", curr, rest, curr->next);
					list_link3(curr, rest, curr->next);
				}
				tprintf("return %p\n", curr + 1);
				return curr + 1;
			}
		}
	}

	HNODE *next = (HNODE*)((char*)curr + len);
	tprintf("grow to %p\n", next + 1);
	grow_mapping_to((unsigned long) (next + 1));
	list_link(curr, next);
	next->next = 0;
	next->allocated = 0;
	curr->allocated = len;
	tprintf("return %p:%d\n", curr, len);
	return curr + 1;
}

void kfree(void *p)
{
	HNODE *curr = (HNODE*)p - 1;
	tprintf("free %p:%d\n", curr, curr->allocated);

	if (curr->next && !curr->next->allocated) {
		list_remove_nextof_s3(curr);
	}

	if (curr->prev && !curr->prev->allocated) {
		list_remove_s2(curr);
	}

	if (!curr->next) {
		curr->prev->next = 0;
		tprintf("shrink to %p\n", curr->prev + 1);
		shrink_mapping_to((unsigned long) (curr->prev + 1));
	}

	curr->allocated = 0;
}
