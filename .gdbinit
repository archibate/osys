# https://github.com/archibate/OS/blob/master/.gdbinit

# Connect to QEMU
target remote localhost:1234

# For bootloader debugging
# change architecture to i386 if debugging STAGE2 32-bit code
#set architecture i8086
set architecture i386
#break *0x7C00

# For kernel debugging
# Add KERNEL file for debugging information
add-symbol-file bin/kernel.elf 0xC0001000

# For both ;)
set disassemble-next-line on
set disassembly-flavor intel