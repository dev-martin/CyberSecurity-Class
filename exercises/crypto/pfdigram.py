#! /usr/bin/env python

import sys

ciphertext = None
with open(sys.argv[1]) as f:
    ciphertext = f.read()

digrams = dict()
for d in ciphertext.split():
    digrams[d] = digrams.get(d,0) + 1

d_entries = [e for e in digrams.items()]
d_entries.sort(key=lambda x: x[1], reverse=True)
d_filt = filter(lambda x: x[1]>1, d_entries)
for d in d_filt:
   print(d)

