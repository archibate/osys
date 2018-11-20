PHONY+=always
always:
	@:


INCLUDE+=. $Dkuinc $Dlib $Duser/ulib
CFLAGS+=$(INCLUDE:%=-I%)

ONAMES:=$(shell cat objs.txt)
OBJS:=$(ONAMES:%=$B%.o)

GCC_LIB+=$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
OBJS+=$(GCC_LIB)


include $Mrules.mak
include $Mmk-bin.mak
include $Mautodep.mak


PRECIOUS+=$B% $B%.o $B%.d

include $Mendup.mak
