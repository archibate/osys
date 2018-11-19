#pragma once

#define E_NO_SRCH 1 // no such file or directory
#define E_INVL_ARG 2 // invalid argument / syscall
#define E_NO_IMPL 3 // not impelemented (yet)
#define E_BAD_FD 4 // bad file descriptor
#define E_NO_WR 5 // not writable (read-only)
#define E_NO_RD 6 // not readable (write-only)
#define E_WRN_TYP 7 // wrong type of inode / spec
#define E_BAD_BLK 8 // bad block / chunk / file
#define E_EXISTD 9 // already existed
#define E_OO_MAX 10 // out of open / slots / memory
#define E_SEG_FL 11 // access violentation / segment fault
