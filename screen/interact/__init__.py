#!/usr/bin/env python3

"""
screen.interact.__init__
Input/Output stream to interact with kernel
"""


from screen.interact import read
from screen.interact import write


def init ():
	read.init ()
	write.init ()
