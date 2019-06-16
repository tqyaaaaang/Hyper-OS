#!/usr/bin/env python3

"""
boot.screen
Boot the screen
"""


import multiprocessing

from screen import __main__ as screen_main
from boot import kernel


def start ():
	screen_main.main (kernel.kern_proc)
