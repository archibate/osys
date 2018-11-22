D=../../
NAME=echo
KTE=exf
#DEFINES+=NO_IN=1

default: all

include $Ddirs.mak
include $Mbeg.mak


PHONY+=all
all: $(IMAGE)


PHONY+=install
install: $(IMAGE)
	cp $< $Nbin/


include $Mend.mak
