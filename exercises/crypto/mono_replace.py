#! /usr/bin/env python

# We're going to provide translations on the command line as "c1=p1
# c2=p2", where ciphertext character c corresponds to plaintext
# character p. This isn't the greatest way to do this, but it makes
# the commandline fairly clear. The first argument will be the
# ciphertext file. Feel free to do this in another way, if you like!

import sys

ciphertext_file = sys.argv[1]

substitutions = dict()
# A slice that only specifies the beginning or end gets everything after
# or before the indices specified.
for a in sys.argv[2:]:
   c,p = a.split('=')
   substitutions[c] = p

ciphertext = None
with open(ciphertext_file) as f:
   ciphertext = f.read()

plaintext = ''.join([substitutions.get(x,x) for x in ciphertext])
print(plaintext)

