#pragma once

#include <sizet.h>
#include "syscall.h"

#ifndef __SYS_STATIC
#ifdef _KERNEL
#define __SYS_STATIC
#else
#define __SYS_STATIC static
#endif
#endif

__SYS_STATIC __attribute__((noreturn)) syscall1x(__builtin_unreachable();,void,exit,(int status), status);
__SYS_STATIC syscall2(int,open,(const char *name, unsigned int oattr), name, oattr);
__SYS_STATIC syscall1(int,close,(int fd), fd);
__SYS_STATIC syscall1(long,tellsize,(int fd), fd);
__SYS_STATIC syscall4(int,mmap,(int fd, void *p, size_t size, unsigned int mattr), fd, p, size, mattr);
__SYS_STATIC syscall3(int,read,(int fd, void *buf, size_t size), fd, buf, size);
__SYS_STATIC syscall3(int,write,(int fd, const void *buf, size_t size), fd, buf, size);
__SYS_STATIC syscall2(int,putch,(int fd, unsigned char ch), fd, ch);
__SYS_STATIC syscall1(unsigned int,getch,(int fd), fd);
__SYS_STATIC syscall1(int,fsync,(int fd), fd);
__SYS_STATIC syscall1(int,setbrk,(void *p), p);
__SYS_STATIC syscall2(int,stexecv1,(const char *name, const char *arg), name, arg);
__SYS_STATIC syscall2(int,waiton,(int pid, unsigned int eveid), pid, eveid);
__SYS_STATIC syscall1(int,chdir,(const char *path), path);
