#!/bin/sh

python makeCollectors.py 50 4 200 > collect_OLPM.c
python makeCollectors.py 92 18 200 > collect_battery.c