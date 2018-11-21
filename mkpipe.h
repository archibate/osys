#pragma once

#include <fs.h>
#include <pipe.h>

INODE *make_pipe_dev(const char *name, PIPE *pipe, unsigned int iattr);
