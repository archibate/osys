#include <schpcb.h>
#include <print.h>


extern
void __attribute__((fastcall)) __switch_from_to
	( PCB *prev
	, PCB *next
	);

void switch_from_to
	( PCB *prev
	, PCB *next
	)
{
	if (0)
	printf("[%p:%p] %s -> [%p:%p] %s\n",
			prev, prev->sp, prev->name ? prev->name : "(noname)",
			next, next->sp, next->name ? next->name : "(noname)");

	setup_pgd(next->pgd);
	__switch_from_to(prev, next);
}
