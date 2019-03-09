# CS50 Problem Set 6
# vigenere
# repeat of past problem
#
# by: Kyle Chatman
# on: 190308

from cs50 import get_string

import sys
import string

# check input is only program name and a string
if len(sys.argv) != 2 or not sys.argv[1].isalpha():
    print("Usage: python vigenere.py k")
    sys.exit(1)

# rename key for brevity, make uppercase, make into list
k = list(sys.argv[1].upper())

# get input and convert to a list for random access
working = list(get_string("plaintext: "))

keylength = len(k)

keyindex = 0

# For each character in string apply vigenere cipher if a to z character
for i in range(len(working)):
    plainascii = ord(working[i])
    keynum = ord(k[keyindex % keylength]) - 65

    # If lowercase
    if plainascii > 96 and plainascii < 123:
        working[i] = chr((plainascii - 97 + keynum) % 26 + 97)
        keyindex += 1

    # If uppercase
    elif plainascii > 64 and plainascii < 91:
        working[i] = chr((plainascii - 65 + keynum) % 26 + 65)
        keyindex += 1

    # If some other character, do nothing

print("ciphertext: {}" .format("".join(working)))