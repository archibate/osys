DISK=a.img

default: run

$(DISK): bootsect.bin kernel.bin
	cat $^ > $@

include tools.mak
include cross.mak
include flags.mak

CFLAGS+=-m32 -fno-stack-protector
LDFLAGS+=-T kernel.ld
ELF=kernel.elf
BIN=kernel.bin
OBJS=$(shell cat objs.txt)

include rules.mak
include rules-bin.mak
include mk-elf.mak
include mk-bin.mak
include mk-run.mak
include endup.mak
