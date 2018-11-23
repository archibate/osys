#pragma once

#include <fs.h>

INODE *procfs_new_proc(TCB *tcb, int pid, unsigned int iattr);
