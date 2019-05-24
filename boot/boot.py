#!/usr/bin/env python3

"""
boot.boot
Boot Hyper-OS
"""


import os
import subprocess

from boot import kernel
from boot import screen


def start ():
	kernel.start ()
	screen.start ()

def stop ():
	kernel.kern_proc.kill ()
	try:
		kernel.kern_proc.wait (timeout=10)
	except subprocess.TimeoutExpired:
		print ('kill failed')
		kernel.kern_proc.terminate ()

	os._exit (0)


def main ():
	start ()

	stop ()
