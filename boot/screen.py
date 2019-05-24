#!/usr/bin/env python3

"""
boot.screen
Boot the screen
"""


import multiprocessing

from screen import __main__ as screen_main


def start ():
	screen_main.main ()
