#! /usr/bin/env python

class CIDR(object):
   def __init__(self, prefix, length):
      self._prefix = prefix
      self._length = length

   def __str__(self):
      return hex(self._prefix) + '/' + str(self._length)

# This class defines our BGP speaker node
class BGP(object):
   def __init__(self, as_num, prefix, length):
      self._num = as_num
      self._block = CIDR(prefix,length)
      self._nbrs = dict()
      self._rtes = dict()
      self._rtes[self._block] = [self._num]

   # We use this to wire the network together
   def add_nbr(self, nbr):
      if nbr._num not in self._nbrs:
         self._nbrs[nbr._num] = nbr
         self._rtes[nbr._block] = [ nbr._num, self._num ]
         nbr.add_nbr(self)

   def receive_update(self,msg):
      for rte in msg:
         dst = rte[0]
         received_path = rte[1]
         path = list()
         # ==> Construct the new path, including this node <==
         path.extend(rte[1]) # This adds an entire list to a list
         # ==> Determine whether to use this path to the destination <==
         if False:
            self._rtes[dst] = path

   # This is what will send/process BGP updates -- you write this!
   def update(self):
      for nbr in self._nbrs:
         nbr = self._nbrs[nbr_num]
         msg = list()
         for block in self._rtes:
            route = self._rtes[block]
            if nbr._num not in route:
               msg.append((block,route))
         nbr.receive_update(msg)

         pass # your code goes here

   # Formatted output -- feel free to change
   def __str__(self):
      s = 'AS: {n}\n   neighbors: {nbrs}\n   routes: {rtes}'.format(
          n=self._num,
          nbrs=self._nbrs.keys(),
          rtes=[str(x)+": "+str(self._rtes[x]) for x in self._rtes]
      )
      return s

# We're creating our network. Don't modify this (at least not for now).
AS = [BGP(x,x<<24,16) for x in range(10)]
AS[0].add_nbr(AS[1])
AS[0].add_nbr(AS[4])
AS[1].add_nbr(AS[2])
AS[1].add_nbr(AS[3])
AS[2].add_nbr(AS[3])
AS[2].add_nbr(AS[5])
AS[3].add_nbr(AS[4])
AS[3].add_nbr(AS[5])
AS[4].add_nbr(AS[8])
AS[5].add_nbr(AS[7])
AS[6].add_nbr(AS[8])
AS[6].add_nbr(AS[9])
AS[7].add_nbr(AS[9])

# This is just to dump the current status.
for a in AS:
   print a
print('')

# You need to define a round of BGP updates, and then run multiple
# rounds to see propagation of routing information.
for x in range(4):
   print('BGP round {}'.format(x))
   for a in AS:
      pass # ==> This is a no-op, replace it appropriately <==
   print('')

