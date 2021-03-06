#pragma once

#ifndef _KERNEL
#include "sysnr.h"
#define __$$SYFN_(rett, x, prot) rett x prot
#define __$$CONCAT(x, y) x##y
#define _$$CONCAT(x, y) __$$CONCAT(x, y)
#define _$SYS(x) _$$CONCAT(_SYS_, x)
#define syscall0(rett, name, prot) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name))); return (rett) __res; }
#define syscall1x(return, rett, name, prot, cx) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name)), "c" (cx)); return (rett) __res; }
#define syscall2(rett, name, prot, cx, dx) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name)), "c" (cx), "d" (dx)); return (rett) __res; }
#define syscall3(rett, name, prot, cx, dx, bx) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name)), "c" (cx), "d" (dx), "b" (bx)); return (rett) __res; }
#define syscall4(rett, name, prot, cx, dx, bx, di) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name)), "c" (cx), "d" (dx), "b" (bx), "D" (di)); return (rett) __res; }
#define syscall5(rett, name, prot, cx, dx, bx, di, si) rett name prot \
	{ long __res; asm volatile ("int $0x80" : "=a" (__res) : "a" (_$SYS(name)), "c" (cx), "d" (dx), "b" (bx), "D" (di), "S" (si)); return (rett) __res; }
#else
#define __$$SYFN(rett, name, prot) rett sys_##name prot
#define syscall0(rett, name, prot) __$$SYFN(rett, name, prot)
#define syscall1x(return, rett, name, prot, cx) __$$SYFN(rett, name, prot)
#define syscall2(rett, name, prot, cx, dx) __$$SYFN(rett, name, prot)
#define syscall3(rett, name, prot, cx, dx, bx) __$$SYFN(rett, name, prot)
#define syscall4(rett, name, prot, cx, dx, bx, di) __$$SYFN(rett, name, prot)
#define syscall5(rett, name, prot, cx, dx, bx, di, si) __$$SYFN(rett, name, prot)
#endif
#define syscall1(rett, name, prot, cx) syscall1x(return, rett, name, prot, cx)
