OPTIMIZE=0
MODES+=32 arch=i386 soft-float
FUNCTIONS+=no-common no-exceptions no-non-call-exceptions \
	  freestanding no-stack-protector no-builtin
WARNINGS+=error all no-error=unused-function
#DEFINES+=TESTING=1

CFLAGS+=-nostdlib -nostdinc
ifdef DEBUG
CFLAGS+=-ggdb -gstabs+
endif
CFLAGS+=$(OPTIMIZE:%=-O%) $(MODES:%=-m%) \
	$(FUNCTIONS:%=-f%) $(WARNINGS:%=-W%) \
	$(DEFINES:%=-D%)
LDFLAGS+=
ASFLAGS+=
NASMFLAGS+=
QEMUFLAGS+=
