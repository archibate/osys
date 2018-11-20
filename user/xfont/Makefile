D=../../
NAME=xfont
KTE=com

DEFINES+=NO_VMON=1

default: all

include $Ddirs.mak
include $Mbeg.mak


PHONY+=all
all: $(IMAGE)


PHONY+=install
install: $(IMAGE)
	cp $< $Nbin/


include $Mend.mak
