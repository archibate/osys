#pragma once

#include <struct.h>
#include <fifo.h>
#include <event.h>

STRUCT(EFIFO)
{
	FIFO fifo;
	EVENT eve_wr;
};

static
fifo_data_t efifo_get(EFIFO *ef)
{
	while (fifo_empty(&ef->fifo))
		wait_on(&ef->eve_wr);
	return fifo_get(&ef->fifo);
}

static
void efifo_put(EFIFO *ef, fifo_data_t data)
{
	fifo_put(&ef->fifo, data);
}

static
void efifo_flush(EFIFO *ef)
{
	trig_up(&ef->eve_wr);
}
