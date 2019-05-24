#!/usr/bin/env python3

"""
boot.kernel
Boot the kernel
"""


import subprocess
import threading
import os
import pathlib


kern_proc = None
kern_up_event = threading.Event ()


def daemon_thread ():
	kern_path = os.path.join (os.path.dirname(os.path.abspath (__file__)), '..', 'bin', 'hos')

	global kern_proc

	kern_proc = subprocess.Popen (
		args = [kern_path],
		stdin = subprocess.PIPE,
		stdout = subprocess.PIPE,
		stderr = subprocess.PIPE
	)

	kern_up_event.set ()

	kern_proc.wait ()
	os._exit (0)


def start ():
	kern_thr = threading.Thread (target=daemon_thread)
	kern_thr.start ()
	kern_up_event.wait ()
