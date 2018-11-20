#pragma once

#include <fs.h>
#include <fifo.h>

INODE *make_fifo_dev(const char *name, FIFO *fifo, unsigned int iattr);
