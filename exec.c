#include <vfs.h>
#include <uload.h>
#include <print.h>
#include <mkproc.h>
#include <mkthrd.h>
#include <kmalloc.h>

int exec(const char *name)
{
	int res = load_user_program(name);
	if (res)
		return res;

	transfer_to_user();
}

int exec_fc(FILE *f)
{
	int res = load_user_program_fc(f);
	if (res)
		return res;

	transfer_to_user();
}


int stexec(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res)
		goto out_free;

	create_thread(create_process((void*)exec_fc, f));

	goto out;
out_free:
	kfree(f);
out:
	return res;
}
