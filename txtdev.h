#pragma once

#include <fs.h>

void setup_txtinfo_dev
	( INODE *inode
	, const void *data
	, size_t size
	);
void setup_strinfo_dev
	( INODE *inode
	, const char *str
	);
extern
FILE_OPS txtdev_fops;
