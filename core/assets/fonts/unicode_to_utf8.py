#!/usr/bin/env python3

import sys

# print('Number of arguments:', len(sys.argv), 'arguments.')
# print('Argument List:', str(sys.argv))

if (len(sys.argv) > 1):
    print(chr(int(str(sys.argv[1]), 16)).encode('utf-8'))
