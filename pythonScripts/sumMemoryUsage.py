#!/usr/bin/env python
import sys

if( len(sys.argv) < 3): 
	print("Should have two arguments")
	exit(0)

f = open(sys.argv[1],"r")
sumRamBytes = 0
for line in f.readlines():
	aux = int(line.split(" ")[0])
	sumRamBytes += aux

f = open(sys.argv[2],"r")
sumFlashBytes = 0
for line in f.readlines():
	aux = int(line.split(" ")[0])
	sumFlashBytes += aux

print "Total of ram memory bytes  : ", sumRamBytes," bytes"
print "Total of flash memory bytes:", sumFlashBytes," bytes"
