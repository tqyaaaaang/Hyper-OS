#!/usr/bin/env python3

"""
boot.kernel
Boot the kernel
"""


import subprocess
import threading
import os


def daemon_thread ():
	files = os.listdir (os.path.join (os.path.dirname(os.path.abspath (__file__)), '..', 'bin'))
	available_files = []
	for i in files:
		if i[0:3] == 'hos':
			available_files.append (i)
	if len(available_files) == 0:
		print ('Cannot find hos kernel')
	elif len(available_files) >= 2:
		print ('Find duplicated hos kernel')
	
	proc = subprocess.Popen (
		args = [os.path.join (os.path.dirname(os.path.abspath (__file__)), '..', 'bin', available_files[0])],
		stdin = subprocess.PIPE,
		stdout = subprocess.PIPE,
		stderr = subprocess.PIPE,
		shell = True
	)

	proc.wait ()
	os._exit (0)


def start ():
	kern_thr = threading.Thread (target=daemon_thread)
	kern_thr.start ()
