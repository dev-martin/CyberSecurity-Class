#! /usr/bin/env python

import sys

# The first argument is the file containing our plaintext.
plaintext = ''
with open(sys.argv[1]) as f:
    # We're going to convert everything to uppercase, but otherwise preserve
    # spaces and punctuation. We'll get rid of line breaks, though.
    for line in f:
        plaintext += line.strip().upper()

# The second argument is the number of rails in our cipher. This is the key.
nrails = int(sys.argv[2])

# Create our initial (empty) rail texts. Try this line in the interactive
# interpreter to see what it does!
rails = [''] * nrails

# This is the length of the rail pattern.
seq_length = 2 * (nrails - 1)

# We're going to map characters to their rails, using a lookup.
rindex = [0] * seq_length
for i in range(0,nrails):
    rindex[i] = i
for i in range(nrails, seq_length):
    rindex[i] = seq_length - i

# Now, let's iterate over the input.
for i in range(len(plaintext)):
    rails[rindex[i%seq_length]] += plaintext[i]

# Concatenate these rails to get our ciphertext.
print(''.join(rails))

# That's it!

