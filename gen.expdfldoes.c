#include <panic.h>
void __attribute__((weak)) do_divide_error
(void)
{
	panic("DIVIDE_ERROR");
}

void __attribute__((weak)) do_debug
(void)
{
	panic("DEBUG");
}

void __attribute__((weak)) do_nmi
(void)
{
	panic("NMI");
}

void __attribute__((weak)) do_break_point
(void)
{
	panic("BREAK_POINT");
}

void __attribute__((weak)) do_overflow
(void)
{
	panic("OVERFLOW");
}

void __attribute__((weak)) do_bounds_check
(void)
{
	panic("BOUNDS_CHECK");
}

void __attribute__((weak)) do_invalid_opcode
(void)
{
	panic("INVALID_OPCODE");
}

void __attribute__((weak)) do_device_not_available
(void)
{
	panic("DEVICE_NOT_AVAILABLE");
}

void __attribute__((weak)) do_double_fault
(void)
{
	panic("DOUBLE_FAULT");
}

void __attribute__((weak)) do_coprocessor_seg_overrun
(void)
{
	panic("COPROCESSOR_SEG_OVERRUN");
}

void __attribute__((weak)) do_invalid_tss
(void)
{
	panic("INVALID_TSS");
}

void __attribute__((weak)) do_segment_not_present
(void)
{
	panic("SEGMENT_NOT_PRESENT");
}

void __attribute__((weak)) do_stack_segment
(void)
{
	panic("STACK_SEGMENT");
}

void __attribute__((weak)) do_general_protection
(void)
{
	panic("GENERAL_PROTECTION");
}

void __attribute__((weak)) do_page_fault
(void)
{
	panic("PAGE_FAULT");
}

void __attribute__((weak)) do_reserved_exception
(void)
{
	panic("RESERVED_EXCEPTION");
}

void __attribute__((weak)) do_coprocessor_error
(void)
{
	panic("COPROCESSOR_ERROR");
}

