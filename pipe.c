#include <pipe.h>
#include <kmalloc.h>
#include <sched.h>

void pipe_init(PIPE *pi)
{
	pi->buf = kmalloc(PIPSIZ);
	pi->reader = 0;
	pi->writer = 0;
	pi->rd = 0;
	pi->wr = 0;
}

void pipe_uninit(PIPE *pi)
{
	kfree(pi->buf);
}


ssize_t pipe_read(PIPE *pi, char *buf, size_t n, int block)
{
	pi->reader = current;

	int i = 0;
	char c;
again:
	while (i < n && pi->rd != pi->wr) {
		buf[i++] = c = pi->buf[pi->rd++];
	}
	if (!block)
		goto out;

	if (i < n) {
		if (pi->writer)
			task_run(pi->writer);
		else
			do_schedule();
		goto again;
	}
out:
	return i;
}

void pipe_write(PIPE *pi, const char *buf, size_t n, int block)
{
	pi->writer = current;

	int i = 0;
again:
	while (i < n && pi->wr != pi->rd - 1) {
		pi->buf[pi->wr++] = buf[i++];
	}
	if (!block)
		goto out;

	if (i < n) {
		if (pi->reader)
			task_run(pi->reader);
		else
			do_schedule();
		goto again;
	}
out:
	return;
}
