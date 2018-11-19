#include <uload.h>

int exec(const char *name)
{
	int res = load_user_program(name);
	if (res)
		goto out;

	transfer_to_user();
out:
	return res;
}

