set -e
flgs="$QEMUFLAGS"
flgs="$flgs -m 256 -serial stdio"
flgs="$flgs -boot a -fda bin/a.img"
if [[ "x$1" = "x-d" ]]; then
	flgs="$flgs -S -gdb tcp::1234"
fi
${QEMU-qemu-system-i386} $flgs
