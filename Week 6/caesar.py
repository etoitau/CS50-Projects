# CS50 Problem Set 6
# caesar
# repeat of past problem
#
# by: Kyle Chatman
# on: 190308

from cs50 import get_string

import sys
import string

# check input is only program name and a positive int
if len(sys.argv) != 2 or not sys.argv[1].isdigit() or int(sys.argv[1]) < 1:
    print("Usage: python caesar.py k")
    sys.exit(1)

# rename key for brevity
k = int(sys.argv[1])

# get input and convert to a list for random access
working = list(get_string("plaintext: "))

# For each character in string apply Caeasr cipher if a to z character
for i in range(len(working)):
    ascii = ord(working[i])
    # If lowercase
    if ascii > 96 and ascii < 123:
        working[i] = chr((ascii - 96 + k) % 26 + 96)
    # If uppercase
    elif ascii > 64 and ascii < 91:
        working[i] = chr((ascii - 64 + k) % 26 + 64)

print("ciphertext: {}" .format("".join(working)))