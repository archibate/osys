#!/usr/bin/env python

exps = '''
timer
keyboard
slavepic
3
serial
5 6 7 8 9 10 11
mouse
13
harddisk
15
'''.split()


def pr_entable_inc():
    wr('\tdd ' + ', '.join('irq' + str(i) for i in range(len(exps))))


def pr_entries_inc():
    for i in range(len(exps)):
        wr('irq' + str(i) + ':')
        wr('\tpush dword ' + str(i))
        wr('\tjmp irq_common')
        wr('')


def pr_entinfo_h():
    wr('#pragma once\n')
    wr('#define IRQ_MAX ' + str(len(exps)))


def pr_idxs_h():
    wr('#pragma once\n')
    for i, exp in enumerate(exps):
        wr('#define IRQ_' + exp.upper() + ' ' + str(i))


def pr_dfldoes_c():
    wr('#include "irq.h" // IRQ_MAX')
    wr('')
    for exp in exps:
        wr('void __attribute__((weak)) do_' + exp)
        wr('(void)')
        wr('{')
        wr('\tirq_done(IRQ_' + exp.upper() + ');')
        wr('}')
        wr('')
    wr('void *irq_table[IRQ_MAX] = {')
    for exp in exps:
        wr('\tdo_' + exp + ',')
    wr('};')


fout = None

def wr(x):
    global fout
    fout.write(x + '\n')


names = '''
entable.inc
entries.inc
entinfo.h
dfldoes.c
idxs.h
'''.split()

for name in names:
    fname = 'gen.irq' + name
    fout = open(fname, 'w')
    print(fname + '...')
    eval('pr_' + name.replace('.', '_') + '()')
