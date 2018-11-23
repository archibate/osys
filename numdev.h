#pragma once

#include <fs.h>

void setup_numinfo_dev
	( INODE *inode
	, unsigned long num
	);
extern
FILE_OPS numdev_fops;
