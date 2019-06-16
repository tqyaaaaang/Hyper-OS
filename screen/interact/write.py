#!/usr/bin/env python3

"""
screen.interact.write
Write service to write to kernel
"""


import threading
import os

from screen import data


def init ():
	pass


def write (val_type, value):
	with data.output_lock:
		data.kern_proc.stdin.write (('{}[{}]\n'.format (val_type, value)).encode ('utf-8'))
		data.kern_proc.stdin.flush ()
