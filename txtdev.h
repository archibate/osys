#pragma once

#include <fs.h>

void setup_txtinfo_dev
	( INODE *inode
	, const char *data
	, size_t size
	);
extern
FILE_OPS txtdev_fops;
