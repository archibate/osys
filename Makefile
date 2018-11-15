-include mkargs.ign

KERN=kernel
KTE=bin

default: all

-include $B.deps


A=z_tools/
S=scripts/
M=makeinc/
B=bin/
N=isodir/


include $Mverbose.mak
include $Mbindir.mak


FDIMG=$Ba.img
BOOTSECT=$Bbootsect.bin
K:=$B$(KERN)
ifndef KERNIMG
KERNIMG=$K.$(KTE)
endif

all: $(FDIMG)


$(FDIMG): $(BOOTSECT) $(KERNIMG)
	dd if=/dev/zero of=$@ bs=$$((1440*1024)) count=1
	mkfs.fat --invariant -F 12 -R 4 -n "OSYS FLOPPY" $@
	dd if=$(BOOTSECT) of=$@ bs=1 skip=62 seek=62 count=$$((512*4-62)) conv=notrunc
	mkdir -p $Bmnt
	-$(SH) $Sumountloop.sh $Bmnt
	$(SH) $Smountloop.sh $Bmnt $@
	sudo cp $(KERNIMG) $Bmnt/kernel.bin
	sudo cp -r isodir/* $Bmnt
	$(SH) $Sumountloop.sh $Bmnt


include $Mcross.mak
include $Mflags.mak


PHONY+=bochsrun
bochsrun: $(FDIMG)
	$(SH) $Sbochs.sh

PHONY+=run
run: $(FDIMG)
	$(SH) $Sqemu.sh

PHONY+=debug
debug: $(FDIMG)
	@#@echo "*** now run 'gdb' in another terminal." >&2
	$(SH) $Sqemu.sh -d

PHONY+=always
always:
	@:


INCLUDE+=. inc lib
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
