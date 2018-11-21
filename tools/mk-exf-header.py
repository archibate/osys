from struct import pack, calcsize
from sys import argv, exit

_, outname, stkpgs, size = argv

with open(outname, 'wb') as fout:
    hdrfmt = 'IIII'
    fout.write(pack(hdrfmt, 0xcafebabe, int(stkpgs), calcsize(hdrfmt), int(size)))
