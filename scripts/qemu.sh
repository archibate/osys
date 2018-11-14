set -e
flgs="$QEMUFLAGS"
flgs="$flgs -m 256"
flgs="$flgs -boot a -fda bin/a.img"
QEMU=${QEMU-qemu-system-i386}
GDB=${GDB-cgdb}

if [[ "x$1" = "x-d" ]]; then
	$QEMU $flgs -S -gdb tcp::1234 &
	$GDB -x qemu.gdb
else
	$QEMU $flgs -serial stdio
fi
