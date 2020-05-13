#! /usr/bin/env python

import sys

# Read railenc.py for explanations for this first part.
ciphertext = ''
with open(sys.argv[1]) as f:
    for line in f:
        ciphertext += line.strip()

nrails = int(sys.argv[2])

rails = [''] * nrails

ciphertext_len = len(ciphertext)

seq_length = 2 * (nrails - 1)

# We'll need to know how many full sequences we have, and how much
# of a partial sequence at the end.
full_seqs = int(ciphertext_len / seq_length)
seq_remainder = ciphertext_len % seq_length

for i in range(0,nrails):
    n = full_seqs
    if i > 0 and i < nrails-1:
        n *= 2
    if seq_remainder > i - 1:
        n += 1
    if seq_remainder > seq_length - i:
        n += 1
    rails[i],ciphertext = ciphertext[0:n],ciphertext[n:]

rindex = [0] * seq_length
for i in range(0,nrails):
    rindex[i] = i
for i in range(nrails, seq_length):
    rindex[i] = seq_length - i

# Now, let's iterate over the input.
plaintext = ''
for i in range(ciphertext_len):
    idx = 0 # ==> Fill this in! <==
    c,rails[idx] = rails[idx][:1], rails[idx][1:]
    plaintext += c

print(plaintext)

