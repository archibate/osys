OUTPUT(i386)
ENTRY(_start)

SECTIONS {
	. = 0x10000000;
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
	.stab : {
		*(.stab)
	}
	.stabstr : {
		*(.stabstr)
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
