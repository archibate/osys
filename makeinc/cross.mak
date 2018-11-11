ifdef WIN
CAT      = type
COPY     = copy
DEL      = del
endif
ifdef A
# cross chain from osask tools
MAKER    = $Amake.exe -r
NASK     = $Anask.exe
CC1      = $Acc1.exe -quiet
GAS2NASK = $Agas2nask.exe -a
OBJ2BIM  = $Aobj2bim.exe
MAKEFONT = $Amakefont.exe
BIN2OBJ  = $Abin2obj.exe
BIM2HRB  = $Abim2hrb.exe
EDIMG    = $Aedimg.exe
IMGTOL   = $Aimgtol.com
GOLIB    = $Agolib00.exe 
QEMU     = $Aqemu.exe
BOCHS    = $Abochs.exe
endif
# standard unix cross chain
CC=$C$(CC_PREFIX)gcc$(CC_SUFFIX)
AS=$C$(CC_PREFIX)as$(CC_SUFFIX)
NASM=$C$(CC_PREFIX)nasm
LD=$C$(CC_PREFIX)ld$(CC_SUFFIX)
OBJCOPY=$C$(CC_PREFIX)objcopy$(CC_SUFFIX)
GDB=$C$(CC_PREFIX)gdb$(CC_SUFFIX)
QEMU=qemu-system-i386#$(shell uname -m)
PYTHON=python
PERL=perl
BOCHS=bochs
DEL=rm -rf
COPY=cp
CAT=cat
SH=sh
