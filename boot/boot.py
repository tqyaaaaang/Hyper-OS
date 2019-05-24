#!/usr/bin/env python3

"""
boot.boot
Boot Hyper-OS
"""


import os

from boot import kernel
from boot import screen


def start ():
	kernel.start ()
	screen.start ()

def stop ():
	os._exit (0)


def main ():
	start ()

	os._exit (0)
