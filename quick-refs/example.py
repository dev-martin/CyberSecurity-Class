#! /usr/bin/env python

import random
import sys

count = int(sys.argv[1])
min = int(sys.argv[2])
max = int(sys.argv[3])

r = random.Random()

nums = [ r.uniform(min,max) for _ in range(count) ]

print('Generated {n} random numbers between {a} and {b}'.format(
   n=count,
   a=min,
   b=max ))
print('Values: {}'.format(nums))

