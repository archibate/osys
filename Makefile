KERN=kernel
KTE=bin

default: run


A=./z_tools/
S=scripts/
M=makeinc/
B=bin/


include $Mbindir.mak


FDIMG=$Ba.img
BOOTSECT=$Bbootsect.bin
K=$B$(KERN)

fdimg: $(FDIMG)


$(FDIMG): $(BOOTSECT) $K.$(KTE)
	dd if=/dev/zero of=$@ bs=$$((1440*1024)) count=1
	cat $^ | dd of=$@ conv=notrunc


include $Mcross.mak
include $Mflags.mak


CFLAGS+=-m32 -fno-stack-protector -I.

OBJS=$(foreach x, $(shell cat objs.txt), $B$x.o)
ASKOBJS=$(OBJS:%=%bj)


include $Mrules.mak
include $Mmk-bin.mak
include $Mmk-run.mak
include $Mendup.mak
