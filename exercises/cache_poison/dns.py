from scapy.all import *

def isDNS(p):
   return DNS in p

def getQuestion(p):
   if not isDNS(p):
      return None
   if p[DNS].qd is None:
      return None
   return p[DNS].qd.qname

def getAnswers(p):
   if not isDNS(p):
      return None
   if p[DNS].an is None:
      return None
   return p[DNS].an

def isResponse(p):
   if not isDNS(p):
      return False
   return p[DNS].qr == 1

if __name__ == '__main__':
   import sys
   infile = sys.argv[1]
   outfile = sys.argv[2]
   inpkts = rdpcap(infile)
   outpkts = list()
   for p in inpkts:
      if isDNS(p):
         if isResponse(p):
            print('This is a response:')
         else:
            print('This is a query:')
         p.show()
         outpkts.append(p)
   empty_query = IP()/DNS()
   outpkts.append(empty_query)
   wrpcap(outfile,outpkts)

