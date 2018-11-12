#include <panic.h>
void __attribute__((weak)) do_divide_error
(void)
{
	panic("UNHANDLED EXCEPTION: DIVIDE_ERROR");
}

void __attribute__((weak)) do_debug
(void)
{
	panic("UNHANDLED EXCEPTION: DEBUG");
}

void __attribute__((weak)) do_nmi
(void)
{
	panic("UNHANDLED EXCEPTION: NMI");
}

void __attribute__((weak)) do_break_point
(void)
{
	panic("UNHANDLED EXCEPTION: BREAK_POINT");
}

void __attribute__((weak)) do_overflow
(void)
{
	panic("UNHANDLED EXCEPTION: OVERFLOW");
}

void __attribute__((weak)) do_bounds_check
(void)
{
	panic("UNHANDLED EXCEPTION: BOUNDS_CHECK");
}

void __attribute__((weak)) do_invalid_opcode
(void)
{
	panic("UNHANDLED EXCEPTION: INVALID_OPCODE");
}

void __attribute__((weak)) do_device_not_available
(void)
{
	panic("UNHANDLED EXCEPTION: DEVICE_NOT_AVAILABLE");
}

void __attribute__((weak)) do_double_fault
(void)
{
	panic("UNHANDLED EXCEPTION: DOUBLE_FAULT");
}

void __attribute__((weak)) do_coprocessor_seg_overrun
(void)
{
	panic("UNHANDLED EXCEPTION: COPROCESSOR_SEG_OVERRUN");
}

void __attribute__((weak)) do_invalid_tss
(void)
{
	panic("UNHANDLED EXCEPTION: INVALID_TSS");
}

void __attribute__((weak)) do_segment_not_present
(void)
{
	panic("UNHANDLED EXCEPTION: SEGMENT_NOT_PRESENT");
}

void __attribute__((weak)) do_stack_segment
(void)
{
	panic("UNHANDLED EXCEPTION: STACK_SEGMENT");
}

void __attribute__((weak)) do_general_protection
(void)
{
	panic("UNHANDLED EXCEPTION: GENERAL_PROTECTION");
}

void __attribute__((weak)) do_page_fault
(void)
{
	panic("UNHANDLED EXCEPTION: PAGE_FAULT");
}

void __attribute__((weak)) do_reserved_exception
(void)
{
	panic("UNHANDLED EXCEPTION: RESERVED_EXCEPTION");
}

void __attribute__((weak)) do_coprocessor_error
(void)
{
	panic("UNHANDLED EXCEPTION: COPROCESSOR_ERROR");
}

