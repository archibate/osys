#include <vfs.h>
#include <uload.h>
#include <sched.h>
#include <umemlay.h>
#include <print.h>
#include <mkproc.h>
#include <memory.h>
#include <string.h>
#include <mkthrd.h>
#include <kmalloc.h>
#include <stddef.h>
#include <umove.h>
#include <pid.h>
#include <mmu.h>
#include <upcb.h>


typedef struct exec_fc_args {
	FILE *f;
	size_t arglen;
	char arg[1];
} exec_fc_args_t;


int execap(const char *name, const char *arg)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res) {
		kfree(f);
		goto out;
	}

	/**/size_t namelen = strlen(name) + 1;
	char *name2 = kmalloc(namelen);
	memcpy(name2, name, namelen);
	current->name = name2;/**/

	size_t arglen = strlen(arg) + 1;
	char *arg2 = kmalloc(arglen);
	memcpy(arg2, arg, arglen);

	// close & kfree for f done in load_user_program_fc()
	res = exec_user_program_fc(f);
	if (res)
		goto out;

	curr_upcb.brk = USER_STACK_BEG;

	unsigned long *sp = (unsigned long*)
		(USER_STACK_END - arglen);
	memcpy(sp, arg2, arglen);
	kfree(arg2);
	*--sp = arglen - 1;

	IF_REGS uregs = {
		.pc = USER_CODE_BEG,
		.sp = (unsigned long) sp,
		.eflags = 0x202,
		.cs = 0x001b,
		.ss = 0x0023,
		.ds = 0x0023,
		.es = 0x0023,
	};

	move_to_user(&uregs);

out:
	return res;
}
