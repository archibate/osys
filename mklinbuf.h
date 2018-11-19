#pragma once

#include <fifo.h>
#include <fs.h>

INODE *make_linebuf_dev
	( const char *name
	, int (*flush_cb)(FIFO *fifo)
	, unsigned int iattr
	);
