# CS50 Problem Set 6
# mario (more)
# repeat of past problem
#
# by: Kyle Chatman
# on: 190307

from cs50 import get_int

while True:
    h = get_int("Height: ")
    if h > 0 and h < 9:
        break

for i in range(h):
    for j in range(h - 1 - i):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for l in range(i + 1):
        print("#", end="")
    print()