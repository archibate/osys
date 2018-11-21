#include <struct.h>
#include <sizet.h>

#define PIPSIZ 65536
typedef unsigned short pipi_t;

STRUCT(PIPE)
{
	char *buf[PIPSIZ];
	pipi_t rd, wr;
}

void pipe_write(PIPE *pip, char *buf, size_t data)
{
	while (pip->wr != pip->rd - 1)
		pip->buf[pip->wr++] = 
}
