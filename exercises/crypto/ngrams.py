#! /usr/bin/env python

# The sys module lets us access command-line arguments
# and exit with an error.
import sys

# Make sure we were given a file to analyze.
if len(sys.argv) < 2:
    print('The ciphertext file must be provided')
    sys.exit(1)

# Lists are 0-indexed, but the first element is the name
# of the program.
ciphertext_file = sys.argv[1]

# A dict is python's map-type class. We create empty ones for 1-,
# 2-, and 3-grams. We can also create these as "{}" instead of
# "dict()".
monograms = dict()
digrams = dict()
trigrams = dict()

# Initialize the ciphertext variable, so that it's in scope.
ciphertext = None

# This is a fairly typical way to open a file. Python uses indentation
# to control scope. By using "with", the file will automatically
# be closed when we leave this block, even if it's due to an error.
with open(ciphertext_file) as f:
    # This will slurp the whole file into our ciphertext variable, which
    # will be a string.
    ciphertext = f.read()

# Now we want to get rid of all whitespace.
ciphertext = ciphertext.replace(' ','').replace('\n','')

# Here's a simple formatted print statement. Most data types can be
# formatted in this way. We add an extra newline for grins.
print('Ciphertext is "{c}"\n'.format(c=ciphertext))

# We're going to do this somewhat inefficiently, looping through
# the ciphertext multiple times.

# This helper function will take a dict named d, and produce a sorted
# list of entries, which it then returns in a printable format.
def freq(d):
    # We can get the keys and values using monograms.items(), but
    # it's not a sortable type, so we're going to use a trivial
    # list comprehension.
    d_entries = [x for x in d.items()]

    # Now we can sort it! We're going to use an anonymous function
    # to do this. Since the entries are (key,value), we want to use
    # the second (0-indexed) item of the entry tuple.
    d_entries.sort(key=lambda x: x[1], reverse=True)

    # Return the results.
    return ['{k} ({v})'.format(k=k,v=v) for k,v in d_entries]

# This is the way you want to iterate through an array-like collection,
# if possible.
for c in ciphertext:
    # We use this instead of monograms[c] += 1 to handle the case
    # where there is no value set yet.
    monograms[c] = monograms.get(c,0) + 1

# This is another way to loop, where we want to reference by index.
for i in range(len(ciphertext)-1):
    # We use a python "slice" operator to pick out 2 characters at
    # a time. This is a special form of array indexing.
    d = ciphertext[i:i+2]
    digrams[d] = digrams.get(d,0) + 1

# ==> Now it's your turn! Implement trigrams. <==

# Sort the n-grams, and print them out in sorted order.
# HINT: Run python interactively, and execute "help(zip)"
for grams in zip(freq(monograms), freq(digrams)):
    print('\t\t'.join(grams))

