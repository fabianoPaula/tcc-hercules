#!/bin/sh

make TARGET=z1 hello-world.ramprof > ./ramprof.txt
sed -i '1d' ramprof.txt >> /dev/null

make TARGET=z1 hello-world.flashprof > ./flashprof.txt
sed -i '1d' flashprof.txt >> /dev/null

python sumMemoryUsage.py
