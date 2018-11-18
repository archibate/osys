#!/usr/bin/env python

exps = '''
divide_error debug nmi break_point overflow
bounds_check invalid_opcode device_not_available
double_fault coprocessor_seg_overrun invalid_tss
segment_not_present stack_segment general_protection
page_fault reserved_exception coprocessor_error
'''.split()


def pr_entable_inc():
    wr('\tdd ' + ', '.join('exp' + str(i) for i in range(len(exps))))


def pr_entriext_inc():
    for exp in exps:
        wr('\tEXTERN do_' + exp)


def pr_entries_inc():
    has_errcd = [8] + list(range(10, 14))

    for i, exp in enumerate(exps):
        wr('exp' + str(i) + ':')
        if i not in has_errcd:
            wr('\tpush dword 0')
        wr('\tpush dword do_' + exp)
        wr('\tjmp exp_common')
        wr('')


def pr_entinfo_h():
    wr('#pragma once\n')
    wr('#define EXP_MAX ' + str(len(exps)))


def pr_dfldoes_c():
    wr('void __attribute__((noreturn)) unhandled_exception(const char *msg);')
    wr('')
    for exp in exps:
        wr('void __attribute__((weak)) do_' + exp)
        wr('(void)')
        wr('{')
        wr('\tunhandled_exception("' + exp.upper() + '");')
        wr('}')
        wr('')


fout = None

def wr(x):
    global fout
    fout.write(x + '\n')


names = '''
entriext.inc
entable.inc
entries.inc
entinfo.h
dfldoes.c
'''.split()

for name in names:
    fname = 'gen.exp' + name
    fout = open(fname, 'w')
    print(fname + '...')
    eval('pr_' + name.replace('.', '_') + '()')
