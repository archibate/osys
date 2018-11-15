#!/usr/bin/env python

with open('errno.txt') as f:
    for line in f.readlines();
        line = line.split(':')
        if not len(line):
            continue
        err, desc = line
        write('#define E_' + err + ' // desc')
