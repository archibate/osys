NAME=kernel
KTE=bin
LDSCRIPT=kernel.ld

default: all

include $Ddirs.mak
include $Mbeg.mak


FDIMG=$Ba.img
BOOTSECT=$Bbootsect.bin

all: $(FDIMG)


APPS=$(shell cat apps.txt)


PHONY+=$A%
$A%:
	make -C $@ install


$(FDIMG): $(BOOTSECT) $(IMAGE) $(APPS:%=$A%)
	dd if=/dev/zero of=$@ bs=$$((1440*1024)) count=1
	mkfs.fat --invariant -S 512 -s 8 -F 12 -R 8 -n "OSYS FLOPPY" $@
	dd if=$(BOOTSECT) of=$@ bs=1 skip=62 seek=62 count=$$((512*4-62)) conv=notrunc
	mkdir -p $Bmnt
	-$(SH) $Sumountloop.sh $Bmnt
	$(SH) $Smountloop.sh $Bmnt $@
	sudo cp $(IMAGE) $Bmnt/kernel.bin
	sudo cp -r isodir/* $Bmnt
	$(SH) $Sumountloop.sh $Bmnt


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


include $Mend.mak
