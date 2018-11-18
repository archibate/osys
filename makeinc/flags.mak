OPTIMIZE=0
MODES+=32 arch=i386 soft-float
FUNCTIONS+=no-common no-exceptions no-non-call-exceptions \
	  freestanding no-stack-protector no-builtin
WARNINGS+=error all no-unused-function no-error=unused-variable \
	  no-error=unused-but-set-variable 
#DEFINES+=TESTING=1#FIXME:bug in dick_func

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
