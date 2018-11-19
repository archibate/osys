#!/usr/bin/env perl

@scs = split ' ', `cat systab.txt`;

open N, '>kuinc/kern/sysnr.h';
open C, '>sysnr.inc';
open T, '>systab.c';

print N <<EOF
#pragma once

#define SYSCALL_MAX ($#scs+1)
EOF
;

print C <<EOF
SYSCALL_MAX equ ($#scs+1)
EOF
;

print T <<EOF
#include <sysnr.h>

EOF
;

foreach $x (@scs) {
	print T "extern int sys_$x();\n";
}

print T <<EOF

int (*syscall_table[SYSCALL_MAX])() = {
EOF
;

$i = 0;
foreach $x (@scs) {
	print C "_SYS_$x equ $i\n";
	print N "#define _SYS_$x $i\n";
	print T "\t[_SYS_$x] = &sys_$x,\n";
	$i++;
}

print T <<EOF
};
EOF
