#! /usr/bin/env python

import random

T = 2
Q = 2*T + 1
N = 3*T + 1

r = random.Random()

values = [0 for _ in range(N)]

# ==> You'll need to account for these elsewhere <==
faulty_nodes = r.sample(range(N), T)

def write_to_quorum():
   print('In write_to_quorum()')
   for w in r.sample(range(N), Q):
      print('  writing to {}'.format(w))
      values[w] = 1

def read_from_quorum():
   print('In read_from_quorum()')
   q = r.sample(range(len(values)), Q)
   for x in q:
      print('  node {} returns {}'.format(x,values[x]))

print('initial values: {}'.format(values))
print('faulty processes: {}'.format(faulty_nodes))

write_to_quorum()
print('new values: {}'.format(values))
read_from_quorum()

