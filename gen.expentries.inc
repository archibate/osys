exp0:
	push dword 0
	push dword do_divide_error
	jmp exp_common

exp1:
	push dword 0
	push dword do_debug
	jmp exp_common

exp2:
	push dword 0
	push dword do_nmi
	jmp exp_common

exp3:
	push dword 0
	push dword do_break_point
	jmp exp_common

exp4:
	push dword 0
	push dword do_overflow
	jmp exp_common

exp5:
	push dword 0
	push dword do_bounds_check
	jmp exp_common

exp6:
	push dword 0
	push dword do_invalid_opcode
	jmp exp_common

exp7:
	push dword 0
	push dword do_device_not_available
	jmp exp_common

exp8:
	push dword do_double_fault
	jmp exp_common

exp9:
	push dword 0
	push dword do_coprocessor_seg_overrun
	jmp exp_common

exp10:
	push dword do_invalid_tss
	jmp exp_common

exp11:
	push dword do_segment_not_present
	jmp exp_common

exp12:
	push dword do_stack_segment
	jmp exp_common

exp13:
	push dword do_general_protection
	jmp exp_common

exp14:
	push dword 0
	push dword do_page_fault
	jmp exp_common

exp15:
	push dword 0
	push dword do_reserved_exception
	jmp exp_common

exp16:
	push dword 0
	push dword do_coprocessor_error
	jmp exp_common

