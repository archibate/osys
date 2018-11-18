void __attribute__((noreturn)) unhandled_exception(const char *msg);

void __attribute__((weak)) do_divide_error
(void)
{
	unhandled_exception("DIVIDE_ERROR");
}

void __attribute__((weak)) do_debug
(void)
{
	unhandled_exception("DEBUG");
}

void __attribute__((weak)) do_nmi
(void)
{
	unhandled_exception("NMI");
}

void __attribute__((weak)) do_break_point
(void)
{
	unhandled_exception("BREAK_POINT");
}

void __attribute__((weak)) do_overflow
(void)
{
	unhandled_exception("OVERFLOW");
}

void __attribute__((weak)) do_bounds_check
(void)
{
	unhandled_exception("BOUNDS_CHECK");
}

void __attribute__((weak)) do_invalid_opcode
(void)
{
	unhandled_exception("INVALID_OPCODE");
}

void __attribute__((weak)) do_device_not_available
(void)
{
	unhandled_exception("DEVICE_NOT_AVAILABLE");
}

void __attribute__((weak)) do_double_fault
(void)
{
	unhandled_exception("DOUBLE_FAULT");
}

void __attribute__((weak)) do_coprocessor_seg_overrun
(void)
{
	unhandled_exception("COPROCESSOR_SEG_OVERRUN");
}

void __attribute__((weak)) do_invalid_tss
(void)
{
	unhandled_exception("INVALID_TSS");
}

void __attribute__((weak)) do_segment_not_present
(void)
{
	unhandled_exception("SEGMENT_NOT_PRESENT");
}

void __attribute__((weak)) do_stack_segment
(void)
{
	unhandled_exception("STACK_SEGMENT");
}

void __attribute__((weak)) do_general_protection
(void)
{
	unhandled_exception("GENERAL_PROTECTION");
}

void __attribute__((weak)) do_page_fault
(void)
{
	unhandled_exception("PAGE_FAULT");
}

void __attribute__((weak)) do_reserved_exception
(void)
{
	unhandled_exception("RESERVED_EXCEPTION");
}

void __attribute__((weak)) do_coprocessor_error
(void)
{
	unhandled_exception("COPROCESSOR_ERROR");
}

