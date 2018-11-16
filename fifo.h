#pragma once

#include <struct.h>

typedef unsigned int fifo_data_t;

STRUCT(FIFO)
{
	fifo_data_t buf[256];
	unsigned char wr, rd; // hiahiahia~ korewa auto-wrap desu
};

static
int fifo_empty(const FIFO *fifo)
{
	return fifo->wr == fifo->rd;
}

static
fifo_data_t fifo_get(FIFO *fifo)
{
	return fifo->buf[fifo->rd++];
}

static
void fifo_put(FIFO *fifo, fifo_data_t data)
{
	fifo->buf[fifo->wr++] = data;
}
