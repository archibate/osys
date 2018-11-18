#pragma once

#include <struct.h>
#include <fifo.h>
#include <sizet.h>
#include <event.h>

STRUCT(EFIFO)
{
	FIFO fifo;
	EVENT eve_wr;
};

static
fifo_idx_t efifo_wait_size(EFIFO *ef)
{
	while (fifo_empty(&ef->fifo))
		wait_on(&ef->eve_wr);
	return fifo_size(&ef->fifo);
}

static
void __efifo_readline(EFIFO *ef, char *buf, size_t size)
{
	while (fifo_empty(&ef->fifo))
		wait_on(&ef->eve_wr);
	while (size--)
		*buf++ = fifo_get(&ef->fifo);
}

static
fifo_data_t efifo_wait_get(EFIFO *ef)
{
	while (fifo_empty(&ef->fifo))
		wait_on(&ef->eve_wr);
	return fifo_get(&ef->fifo);
}

static inline
void efifo_put(EFIFO *ef, fifo_data_t data)
{
	fifo_put(&ef->fifo, data);
}

static inline
void efifo_flush(EFIFO *ef)
{
	trig_up(&ef->eve_wr);
}