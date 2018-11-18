PHONY+=always
always:
	@:


INCLUDE+=. inc lib
CFLAGS+=$(INCLUDE:%=-I%)

ONAMES:=$(shell cat objs.txt)
OBJS:=$(ONAMES:%=$B%.o)

GCC_LIB+=$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
OBJS+=$(GCC_LIB)


include $Mrules.mak
include $Mmk-bin.mak
include $Mautodep.mak


PRECIOUS+=$B%

include $Mendup.mak