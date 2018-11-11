KERN=kernel
KTE=bin

default: all

-include $B.deps


A=z_tools/
S=scripts/
M=makeinc/
B=bin/


include $Mverbose.mak
include $Mbindir.mak


FDIMG=$Ba.img
BOOTSECT=$Bbootsect.bin
K:=$B$(KERN)

all: $(FDIMG)


$(FDIMG): $(BOOTSECT) $K.$(KTE)
	dd if=/dev/zero of=$@ bs=$$((1440*1024)) count=1
	cat $^ | dd of=$@ conv=notrunc


include $Mcross.mak
include $Mflags.mak


PHONY+=bochsrun
bochsrun: $(FDIMG)
	$(SH) bochs.sh

PHONY+=run
run: $(FDIMG)
	$(SH) qemu.sh

PHONY+=debug
debug: $(FDIMG)
	@echo "*** now run 'gdb' in another terminal." >&2
	$(SH) qemu.sh -d

PHONY+=always
always:
	@:


INCLUDE+=. inc libc
CFLAGS+=$(INCLUDE:%=-I%)

ONAMES:=$(shell cat objs.txt)
OBJS:=$(ONAMES:%=$B%.o)
ASKOBJS:=$(OBJS:%=%bj)

GCC_LIB+=$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
OBJS+=$(GCC_LIB)


include $Mrules.mak
include $Mmk-bin.mak
include $Mautodep.mak


PRECIOUS+=$B%

include $Mendup.mak
