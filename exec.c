#include <vfs.h>
#include <uload.h>
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


typedef struct exec_fc_args {
	FILE *f;
	size_t arglen;
	char arg[1];
} exec_fc_args_t;


static
int exec_fc(exec_fc_args_t *earg)
{
	// close & kfree for f done in load_user_program_fc()
	int res = load_user_program_fc(earg->f);
	if (res)
		return res;

	unsigned long *sp = (unsigned long*)
		(USER_STACK_END - earg->arglen - 1);
	memcpy(sp, earg->arg, earg->arglen);
	*--sp = earg->arglen;
	kfree(earg);

	IF_REGS uregs = {
		.pc = USER_TEXT_BEG,
		.sp = (unsigned long) sp,
		.eflags = 0x202,
		.cs = 0x001b,
		.ss = 0x0023,
		.ds = 0x0023,
		.es = 0x0023,
	};
	move_to_user(&uregs);
}


int stexecv1(const char *name, const char *arg)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res) {
		kfree(f);
		return res;
	}

	size_t arglen = strlen(arg);
	exec_fc_args_t *earg = kmalloc(offsetof(exec_fc_args_t, arg) + arglen);
	memcpy(earg->arg, arg, arglen);
	earg->arglen = arglen;
	earg->f = f;

	// close & kfree in exec_fc()
	res = new_proc(create_thread(create_process((void*)exec_fc, earg)));

	return res;
}
