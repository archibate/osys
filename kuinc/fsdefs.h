#pragma once


#define MAX_FNAME 30

// inode.i_type:
#define INODE_FIL 0
#define INODE_DIR 1
#define INODE_CHR 2
#define INODE_BLK 3
// inode.i_attr:
#define INODE_RD  (1<<(8+0))
#define INODE_WR  (1<<(8+1))
#define INODE_EX  (1<<(8+2))
// inode.i_fat.ie_attr
#define FAT_RDONLY (1<<0)
#define FAT_HIDDEN (1<<1)
#define FAT_SYSTEM (1<<2)
#define FAT_VOLLAB (1<<3)
#define FAT_SUBDIR (1<<4)
#define FAT_ARCHIV (1<<5)
// open.oattr:
#define OPEN_RD  (1<<0)
#define OPEN_WR  (1<<1)
#define OPEN_DIR (1<<2)
// seek.whence:
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
// mmap.mattr
#include "kern/page.h" // PG_*
#define MMAP_WR  PG_W
#define MMAP_USR PG_U
