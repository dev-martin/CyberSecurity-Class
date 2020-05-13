#! /usr/bin/env python

import sys
import hashlib

# A Feistel cipher using hashes requires four functions. You can
# hard-code the keys, or read them from the command line.

# To make things simple, we'll have a 2-byte block, with Left and Right
# being single bytes.

block_size = 2
input_text = sys.argv[1]
if 0 != len(input_text)%2:
    input_text += 'X'

# We're going to use splices and zip to put together our blocks, already
# separated into Left and Right.  The splice operator [0::2] acts like [0:],
# in that it starts with index 0 and takes everything to the end. However,
# the added ":2" adds a step increment. The full specification for a splice
# would be [start:stop:step]. Zip takes multiple iterables and assembles them
# into tuples of the corresponding elements.
blocks = zip( input_text[0::2], input_text[1::2] )

# ==> Edit these functions <==
def f1(x):
    return x

def f2(x):
    return x

def f3(x):
    return x

def f4(x):
    return x

# Create empty lists for the plaintext and ciphertext, as numbers.
input_blocks = list()
cipher_blocks = list()
for block in blocks:
    l1 = ord(block[0])
    r1 = ord(block[1])
    input_blocks.append((l1 << 8) + r1)

    l2 = r1
    r2 = l1 ^ f1(r1)

    l3 = r2
    r3 = l2 ^ f2(r2)

    l4 = r3
    r4 = l3 ^ f3(r3)

    l5 = r4
    r5 = l4 ^ f4(r4)

    c = (r5 << 8) + l5
    cipher_blocks.append(c)

print('Input\tOutput')
print('------\t------')
for p,c in zip(input_blocks,cipher_blocks):
    print('{0:04x}\t{1:04x}'.format(p,c))

