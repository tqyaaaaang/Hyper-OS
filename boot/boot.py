#!/usr/bin/env python3

"""
boot.boot
Boot Hyper-OS
"""


from boot import kernel
from boot import screen


def start ():
	kernel.start ()
	screen.start ()


def main ():
	start ()
