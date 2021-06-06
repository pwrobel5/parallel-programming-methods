#!/usr/bin/env python
"""sequential.py"""

import sys
from os import listdir
from os.path import isfile, join

if len(sys.argv) != 2:
    print("Invalid number of initial arguments given")
    exit()

input_dir = sys.argv[1]
input_files = [join(input_dir, f) for f in listdir(input_dir) if isfile(join(input_dir, f))]
results = {}

for input_file_name in input_files:
    input_file = open(input_file_name, "r")

    for line in input_file:
        line = line.strip().lower().split()
        for word in line:
            if word in results:
                results[word] += 1
            else:
                results[word] = 1

    input_file.close()

for key in sorted(results, key=results.get, reverse=True):
    print("({}, {})".format(key, results[key]))

