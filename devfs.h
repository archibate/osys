#pragma once

#include <fs.h>

INODE *register_dev
	( FILE_OPS *fops
	, const char *name
	, unsigned int iattr
	);
