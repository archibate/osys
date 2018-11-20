#pragma once

#include <fs.h>

INODE *make_txtinfo_dev
	( const char *name
	, const void *data
	, size_t size
	);
