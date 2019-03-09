# CS50 Problem Set 6
# bleep
# user gives text file with banned words in it
#   all lowercase, one word per line
#
# by: Kyle Chatman
# on: 190308
#
# Spec:
#   don't ban big words that just contain a banned word (e.g. darn is banned but darnit is not)
#   can assume user will not input punctuation, just words and whitespace

from cs50 import get_string
from sys import argv, exit
import string

words = set()


def main():

    # Check for correct number of args
    if len(argv) != 2:
        print("Usage: bleep.py dictionary")
        exit(1)

    # Load dictionary into memory
    if not load(argv[1]):
        print("Could not load {}" .format(argv[1]))
        exit(1)

    # get input from user and tokenize by splitting at spaces
    splittext = get_string("What message would you like to censor?\n").split()

    # go through words censoring words in banned word list and printing word or *** as appropriate
    for word in splittext:
        if word.lower().strip() in words:
            for i in range(len(word)):
                print("*", end='')
            print(" ", end='')
        else:
            print(word + " ", end='')
    print()


def load(dictionary):
    # Load dictionary into memory, returning true if successful else false"""
    file = open(dictionary, "r")
    for line in file:
        words.add(line.rstrip("\n"))
    file.close()
    return True


if __name__ == "__main__":
    main()
