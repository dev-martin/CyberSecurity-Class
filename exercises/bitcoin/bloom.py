#! /usr/bin/env python

import hashlib
import sys

n = int(sys.argv[1])
k = int(sys.argv[2])

filter = [False] * n
hs = [ (lambda y:
        lambda x: int(hashlib.sha1(str(y) + str(x)).hexdigest(),16) % n)(a)
       for a in range(k) ]

print "Filter: {f}".format(f=filter)

while True:
    l = None
    try:
        l = sys.stdin.readline()
    except:
        break
    if not l:
        break
    item = ''.join(l[1:])
    if '+' == l[0]:
        # ==> Put your add-to-filter code here <==
        pass
    elif '?' == l[0]:
        found = True
        # ==> Put your test-in-filter code here <==
        if found:
            print "Item in filter"
        else:
            print "Item not in filter"
    else:
        print "Unknown command, must be '+<item>' or '?item'"
    print "Filter: {f}".format(f=filter)
    sys.stdout.flush()

