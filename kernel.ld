OUTPUT(i386)
ENTRY(_start)

SECTIONS {
	. = 0x100000;
	.text : {
		*(.text*)
		*(.stext*)
		*(.rdata*)
		*(.rodata*)
	}
	.data : {
		*(.data*)
		*(.sdata*)
	}
	.bss : {
		*(.bss*)
		*(.sbss*)
		*(COMMON)
	}
	/DISCARD/ : {
		/**(.eh_frame*)
		*(.note*)
		*(.comment*)
		*(.GNU*)
		*(.gnu*)
		*(.dyn*)
		*(.interp*)*/
	}
}
