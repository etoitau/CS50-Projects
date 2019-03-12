# CS50 Problem Set 7
# similarities
# Functions used to compare strings
# helpers.py and index.html by me, rest is distribution code
#
# by: Kyle Chatman
# on: 190312

from nltk.tokenize import sent_tokenize
#from string import split, rstrip


def lines(a, b):
    """Return lines in both a and b"""
    # Spec
    #   can assume lines are separated by \n, but we need to strip that
    #   do need to count a blank line as a match and add to list as ""
    #   No duplicates
    #   if blank, than ""

    wordsa = list(a.split('\n'))
    wordsb = list(b.split('\n'))
    wordsm = list()
    for linea in wordsa:
        if linea not in wordsm:
            if linea in wordsb:
                wordsm.append(linea)
    return wordsm


def sentences(a, b):
    """Return sentences in both a and b"""
    # Spec
    #   use sent_tokenize from natrual language toolkit to separate ea string into list
    #   no duplicates
    wordsa = list(sent_tokenize(a))
    wordsb = list(sent_tokenize(b))
    wordsm = list()
    for sent in wordsa:
        if sent not in wordsm:
            if sent in wordsb:
                wordsm.append(sent)
    return wordsm


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    #   no duplicates
    wordsm = list()
    length = len(a)
    for i in range(n, length + 1):
        sub = a[i - n: i]
        if sub not in wordsm:
            if sub in b:
                wordsm.append(sub)
    return wordsm