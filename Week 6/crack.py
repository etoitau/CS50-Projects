# CS50 Problem Set 6
# crack
# repeat of past problem
#
# by: Kyle Chatman
# on: 190308
#
# Spec:
#   Password may be up to 5 characters
#   Password will be upper or lowercase letters only - can assume
#
# known solutions:
#   50kiZhZaSRp0E is ab
#   51.xJagtPnb6s is TF
#   50GApilQSG3E2 is UPenn
#   502sDZxA/ybHs is puppy
#   50C6B0oz0HWzo is FTW
#   50WUNAFdX/yjA is Yale
#   50n0AAUD.pL8g is lloyd
#   50CcfIk1QrPr6 is maybe
#   50JIIyhDORqMU is nope
#   51v3Nh6ZWGHOQ is ROFL
#   61v1CDwwP95bY is hola
#   508ny6Rw0aRio is sean
#   50cI2vYkF0YU2 is LOL

# max possible password length (spec says 5)
maxlength = 5

from cs50 import get_string

import sys
import string
import crypt


def main():
    # check input is only program name and a positive int
    if len(sys.argv) != 2:
        print("Usage: python crack.py hash")
        sys.exit(1)

    # get salt
    salt = sys.argv[1][0:2]

    # use recursive function to generate and check all combinations of length i
    for i in range(maxlength):
        guess = list('a' * (i+1))
        if brute(i+1, guess, salt):
            break

# recursive function to genereate and check all combos and check


def brute(level, guess, salt):
    if level == 0:
        return 0
    for c in list(string.ascii_letters):
        # change one letter
        guess[level - 1] = c
        # try guess
        if sys.argv[1] == crypt.crypt("".join(guess), salt):
            print("".join(guess))
            return 1
        # go deeper
        if brute(level - 1, guess, salt):
            return 1


if __name__ == "__main__":
    main()