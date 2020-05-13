from scapy.all import *

pkt = IP(dst='192.168.0.12')/ICMP()
pkt.display()
for i in range(100):
    send(pkt)
