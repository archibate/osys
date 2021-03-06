# vim: ft=gdb ts=2 sts=2 sw=2
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
add-symbol-file bin/kernel.elf 0x100000
#add-symbol-file user/cat/bin/cat.elf 0x10000000
#add-symbol-file user/die/bin/die.elf 0x10000000
add-symbol-file user/xfont/bin/xfont.elf 0x10000000
#add-symbol-file user/echo/bin/echo.elf 0x10000000
#add-symbol-file user/shell/bin/shell.elf 0x10000000
#add-symbol-file user/gview/bin/gview.elf 0x10000000
#add-symbol-file user/hello/bin/hello.elf 0x10000000
#add-symbol-file user/true/bin/true.elf 0x10000000
#add-symbol-file user/exectest/bin/exectest.elf 0x10000000
#add-symbol-file user/ls/bin/ls.elf 0x10000000

# For both ;)
set disassemble-next-line on
set disassembly-flavor intel

define qq
	kill
	quit
end

define bc
	break $arg0
	continue
end

define xs
	x/6wx $esp
end

define xsi
	x/10i *(void**)($esp+4)
end

b panic
b exp14
b do_break_point
#bc panic
#bc main
#bc set_break
#bc load_user_program_fc
#bc pipe_read
#bc fgets
#bc __crt_start
#c
#c
#b my_buf_putc
#bc fprintf.c:25
#define cp
  #c
  #p c
#end
#bc sys_stexecv1
#b exec_fc
#bc main.c:72
#bc sys_execap
#bc _start
#b l2_copy_pages
#b l1_copy_pages
#bc sys_fork
#b fork.c:41
#bc forked_entry_fc
#bc sys_execap
#bc _start
#bc sys_exit
#bc pexit.c:17
#bc fork.c:106
#bc int_return
#bc sys_fork
#bc sys_fork
#bc __crt_start
#c
#bc forked_entry_fc
#bc int_return
#bc __crt_start
#bc crt.c:45
#bc free
#bc __crt_start
#bc __crt_start
#c
#bc user_space_copy_to
#b init_heap
#bc run_cmd
#bc sys_execap
#bc exec_user_program_fc
#bc __crt_start
#c
#bc execap
#bc exec.c:60
#bc schpcb.c:17
#c
#c
#c
#bc kmalloc
#bt
#bc mkproc.c:47
#bc __syscall_entry
#bc _start
#bc sys_map_upcb
#bc __crt_start
#c
#bc main
#bc fclose
#bc init_welcome
#bc __crt_start
#c
#b ramfs_open_inode
#b dirfind
#b ramfs.c:224
#bc chdir
#bc inode_open
#bc ramfs_decode_fatents
#c
#bc ramfs.c:427
#c
#bt
#xs
#xsi
#bc execap
#bc exec_user_program_fc
#bc uload.c:39
#bc ramfs_read
#bc ramfs.c:73
#bc __crt_start
#bc inode_open
#bc free
#bc __crt_start
#c
#c
#b on_keyboard_event
#bc sys_open
bc __crt_start
bc main.c:189
#bc main
#bc vmon_recursor
