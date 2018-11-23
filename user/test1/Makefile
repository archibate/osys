D=../../
NAME=test1
KTE=exf

default: all

include $Ddirs.mak
include $Mbeg.mak


PHONY+=all
all: $(IMAGE)


PHONY+=install
install: $(IMAGE)
	cp $< $Nbin/


include $Mend.mak
