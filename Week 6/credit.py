# CS50 Problem Set 6
# credit
# repeat of past problem
#
# by: Kyle Chatman
# on: 190307

from cs50 import get_string

# get input and reject if bad length or not number
while True:
    cnum = get_string("Number: ")
    length = len(cnum)
    if length < 17 and length > 0 and cnum.isdigit():
        break

# do Luhn algorithm check
cksum = 0
for i in range(length):
    dig = int(cnum[length - 1 - i])
    if i % 2 == 0:
        cksum += dig
    else:
        if dig < 5:
            cksum += 2 * dig
        else:
            cksum += 1 + 2 * dig % 10

# if luhn looks okay, check prefix to type card
if cksum % 10 == 0:
    pre = int(cnum[0:2])
    if pre == 34 or pre == 37:
        print("AMEX")
    elif pre > 50 and pre < 56:
        print("MASTERCARD")
    elif pre > 39 and pre < 50:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")

