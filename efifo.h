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

static inline
void efifo_init(EFIFO *ef)
{
	fifo_init(&ef->fifo);
	event_init(&ef->eve_wr);
}

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

/*static
fifo_data_t efifo_wf_get(EFIFO *ef)
{
	if (fifo_empty(&ef->fifo))
		return wait_on(&ef->eve_wr);
	return fifo_get(&ef->fifo);
}*/

static inline
void efifo_put(EFIFO *ef, fifo_data_t data)
{
	fifo_put(&ef->fifo, data);
}

static inline
void efifo_flush(EFIFO *ef)
{
	trig_up(&ef->eve_wr/*, 0x80*/);
}

static inline
int __efifo_flush(EFIFO *ef)
{
	return __trig_up(&ef->eve_wr/*, 0x80*/);
}

/*static inline
void efifo_wf_flush(EFIFO *ef)
{
	trig_up(&ef->eve_wr, fifo_get(&ef->fifo));
}*/

/*static inline
void efifo_wf_put_flush(EFIFO *ef, fifo_data_t data) // wf-system remains todo
{
	if (!fifo_empty(&ef->fifo)) {
		fifo_put(&ef->fifo, data);
		data = 0x80;
	}
	trig_up(&ef->eve_wr, data);
}*/
