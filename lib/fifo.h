#pragma once

#include <struct.h>

typedef unsigned int fifo_data_t;
typedef unsigned char fifo_idx_t;
#define FIFO_MAX 256
#define fifo_workwell(fifo) (fifo_size(fifo) <= FIFO_MAX - 1)

STRUCT(FIFO)
{
	fifo_data_t buf[FIFO_MAX];
	fifo_idx_t wr, rd; // hiahiahia~ korewa auto-wrap desu
};

static inline
int fifo_empty(const FIFO *fifo)
{
	return fifo->wr == fifo->rd;
}

static inline
void fifo_init(FIFO *fifo)
{
	fifo->wr = fifo->rd = 0;
}

static inline
fifo_idx_t fifo_size(const FIFO *fifo)
{
	return fifo->wr - fifo->rd;
}

static inline
fifo_data_t fifo_get(FIFO *fifo)
{
	return fifo->buf[fifo->rd++];
}

static inline
void fifo_put(FIFO *fifo, fifo_data_t data)
{
	fifo->buf[fifo->wr++] = data;
}
