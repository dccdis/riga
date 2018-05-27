#!/usr/bin/python
'''
Usage:
argv[1] = accounts file
argv[2] = max password length to iterate
'''

import sys
import string
from hashlib import md5


def passwdgenerator(maxlength):
    validchars = string.printable[:94]
    if (maxlength == 0):
        yield tuple()
        return
    for x in validchars:
        for y in passwdgenerator(maxlength-1):
            yield tuple(x)+y


def parseaccounts(accountfile):
    accounts = []
    for line in open (accountfile, 'r'):
        (username, salt, hashvalue) = line.split('|')
        accounts.append((hashvalue.strip(), str(int(salt, 16)), username))
    return accounts


accounts = parseaccounts(sys.argv[1])
pwmaxlen = 1 + int(sys.argv[2])
for length in range (0, pwmaxlen):
    for i in passwdgenerator(length):    
        password = ''.join(i)
        for nextuser in accounts:
            (hashvalue,salt,username) = nextuser
            h = md5()
            h.update(password)
            h.update(salt)
            if (h.hexdigest() == hashvalue):
                print "Found password: \t" + username + "/" + password

