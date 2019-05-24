#!/usr/bin/env python3

"""
screen.__main__
"""

from screen import data
from screen import screen


def main (kern_proc):
	data.kern_proc = kern_proc

	screen.init ()

	screen.run ()
