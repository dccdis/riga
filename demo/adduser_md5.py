#!/usr/bin/python
'''
Simple adduser script for testing
'''

from hashlib import md5 as used_algorithm
import random
import sys
import getpass

def generate_salt():
    return str(random.getrandbits(32))
    
def hashfunc(password, salt):
    h = used_algorithm()
    h.update(password)
    h.update(salt)
    return h.hexdigest()

def adduserprompt():
    print
    print "Adding new user:"
    username = raw_input("Enter username:")
    ready = False
    while not ready:
        password = getpass.getpass("Enter password:")
        confirm_password = getpass.getpass("Re-enter password for confirmation:")
        if (password == confirm_password):
            ready = True
        else:
            print "Passwords do not match!"
            
    print "User added"
    return (username.strip(), password.strip())
    
    
username, password = adduserprompt()
salt = generate_salt()
passwordhash = hashfunc (password, salt)
authfile = open("./md5testaccounts", "a")
authfile.write ((username + "|%x|" + passwordhash + "\n") % int(salt))

