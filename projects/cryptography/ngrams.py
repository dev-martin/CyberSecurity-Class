#! /usr/bin/env python

# The sys module lets us access command-line arguments
# and exit with an error.
import sys
import math
from collections import defaultdict

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

sixgrams =   dict()
digrams =   dict()
septgrams =  dict()
octgrams =   dict()
ninegrams =  dict()

sixgrams_entries =  dict()
digrams_entries =  dict()
septgrams_entries = dict()
octgrams_entries =  dict()
ninegrams_entries = dict()

sixgrams_distances =  dict()
digrams_distances =  dict()
septgrams_distances = dict()
octgrams_distances =  dict()
ninegrams_distances = dict()

final_distances = list()

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
    # rreturn ['{k} ({v})'.format(k=k,v=v) for k,v in d_entries]
    return ['{k}'.format(k=k) for k,v in d_entries]


# def compute_gcd(x,y):
    # while(y):
        # x, y = y, x % y
    # return x

# # 2-grams
# for i in range(len(ciphertext)-1):
    # digram = ciphertext[i:i+1]
    # digrams[digram] = digrams.get(digram,0) + 1
    # if digrams[digram] != 0:
        # if digrams[digram] == 1:
            # digrams_entries[digram] = list()
        # digrams_entries[digram].append(i)

# # 6-grams
# for i in range(len(ciphertext)-5):
    # sixgram = ciphertext[i:i+6]
    # sixgrams[sixgram] = sixgrams.get(sixgram,0) + 1
    # if sixgrams[sixgram] != 0:
        # if sixgrams[sixgram] == 1:
            # sixgrams_entries[sixgram] = list()
        # sixgrams_entries[sixgram].append(i)

# # 7-grams
# for i in range(len(ciphertext)-6):
    # septgram = ciphertext[i:i+7]
    # septgrams[septgram] = septgrams.get(septgram,0) + 1
    # if septgrams[septgram] != 0:
        # if septgrams[septgram] == 1:
            # septgrams_entries[septgram] = list()
        # septgrams_entries[septgram].append(i)

# # 8-grams
# for i in range(len(ciphertext)-7):
    # octgram = ciphertext[i:i+8]
    # octgrams[octgram] = octgrams.get(octgram,0) + 1
    # if octgrams[octgram] != 0:
        # if octgrams[octgram] == 1:
            # octgrams_entries[octgram] = list()
        # octgrams_entries[octgram].append(i)

# # 9-grams
# for i in range(len(ciphertext)-8):
    # ninegram = ciphertext[i:i+9]
    # ninegrams[ninegram] = ninegrams.get(ninegram,0) + 1
    # if ninegrams[ninegram] != 0:
        # if ninegrams[ninegram] == 1:
            # ninegrams_entries[ninegram] = list()
        # ninegrams_entries[ninegram].append(i)

# # Calculate distances
# for digram in digrams:
    # if digrams[digram] > 1:
        # digrams_distances[digram] = digrams_entries[digram][1]-digrams_entries[digram][0]
        # if digrams_distances[digram]  not in final_distances:
            # final_distances.append(digrams_distances[digram])
# for sixgram in sixgrams:
    # if sixgrams[sixgram] > 1:
        # sixgrams_distances[sixgram] = sixgrams_entries[sixgram][1]-sixgrams_entries[sixgram][0]
        # if sixgrams_distances[sixgram]  not in final_distances:
            # final_distances.append(sixgrams_distances[sixgram])
# for septgram in septgrams:
    # if septgrams[septgram] > 1:
        # septgrams_distances[septgram] = septgrams_entries[septgram][1]-septgrams_entries[septgram][0]
        # if septgrams_distances[septgram]  not in final_distances:
            # final_distances.append(septgrams_distances[septgram])
# for octgram in octgrams:
    # if octgrams[octgram] > 1:
        # octgrams_distances[octgram] = octgrams_entries[octgram][1]-octgrams_entries[octgram][0]
        # if octgrams_distances[octgram]  not in final_distances:
            # final_distances.append(octgrams_distances[octgram])
# for ninegram in ninegrams:
    # if ninegrams[ninegram] > 1:
        # ninegrams_distances[ninegram] = ninegrams_entries[ninegram][1]-ninegrams_entries[ninegram][0]
        # if ninegrams_distances[ninegram]  not in final_distances:
            # final_distances.append(ninegrams_distances[ninegram])

# # Sort the n-grams, and print them out in sorted order.
# print("Digrams:")
# for digram in freq(digrams):
    # if digrams[digram] > 1:
        # print('{k} ({v}) Distance: {l}'.format(k=digram,v=digrams[digram],l=digrams_distances[digram]))
# print(" ")
# print("Sixgrams:")
# for sixgram in freq(sixgrams):
    # if sixgrams[sixgram] > 1:
        # print('{k} ({v}) Distance: {l}'.format(k=sixgram,v=sixgrams[sixgram],l=sixgrams_distances[sixgram]))
# print(" ")
# print("Septgrams:")
# for septgram in freq(septgrams):
    # if septgrams[septgram] > 1:
        # print('{k} ({v}) Distance: {l}'.format(k=septgram,v=septgrams[septgram],l=septgrams_distances[septgram]))
# print(" ")
# print("Octgrams:")
# for octgram in freq(octgrams):
    # if octgrams[octgram] > 1:
        # print('{k} ({v}) Distance: {l}'.format(k=octgram,v=octgrams[octgram],l=octgrams_distances[octgram]))
# print(" ")
# print("Ninegrams:")
# for ninegram in freq(ninegrams):
    # if ninegrams[ninegram] > 1:
        # print('{k} ({v}) Distance: {l}'.format(k=ninegram,v=ninegrams[ninegram],l=ninegrams_distances[ninegram]))
# print(" ")
# print(" ")
# print("Useful distances to do GCD: " + str(final_distances))

# gcd = compute_gcd(final_distances[0],final_distances[1])
# for i  in range( 2, len(final_distances)):
   # gcd = compute_gcd(gcd,final_distances[i])

# print("Keyword length: " + str(gcd) +  "\n")

# # Find out number of ocurrences of each letter in each of the <gcd> columns
first_column = dict()
second_column =dict()

# Create string of first column
for i in range(0,len(ciphertext) - 2, 2):
    first_column[ciphertext[i]] = first_column.get(ciphertext[i],0) + 1

# Create string of second column
for i in range(1, len(ciphertext) - 2, 2):
    second_column[ciphertext[i]] = second_column.get(ciphertext[i],0) + 1

print( first_column)
print( second_column)
# Findout most common letter in each column and mapped to E
cipherword = ''

first_list  = [(value, key) for key, value in first_column.items()]
cipherword += max(first_list)[1]

second_list  = [(value, key) for key, value in second_column.items()]
cipherword += max(second_list)[1]

# third_list  = [(value, key) for key, value in third_column.items()]
# cipherword += max(third_list)[1]

# fourth_list  = [(value, key) for key, value in fourth_column.items()]
# cipherword  += max(fourth_list)[1]

print("Cipherword: " + cipherword)

# Now, map it to E
keyword = ''
keyword += chr( (ord(cipherword[0]) - ord('e')) + 97)
keyword += chr( (ord(cipherword[1]) - ord('e')) + 97)
# keyword += chr( (ord(cipherword[2]) - ord('e')) + 97)
# keyword += chr( (ord(cipherword[3]) - ord('e')) + 123)

print("Keyword: " + keyword)

#Output to a file so decrypt.c can work with it
output = open("keyword1.txt", "w")
output.write(keyword)

