#!/usr/bin/env python3

"""
screen.__main__
"""

from screen import data
from screen import screen
from screen import interact


def main (kern_proc):
	data.kern_proc = kern_proc

	interact.init ()
	screen.init ()

	screen.run ()
