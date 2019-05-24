#!/usr/bin/env python3

"""
screen.interact.read
Read service to read from kernel
"""


import threading
import os

from screen import data
from screen.output import write


read_thr = None


def init ():
	read_thr = threading.Thread (target=read_thread_entry)
	read_thr.start ()
	data.messages_window.buffer.insert_text ('h')


def read_thread_entry ():
	data.screen_up_event.wait ()
	while not data.application.is_running:   # Wait for the applicate to run
		pass
	for line in data.kern_proc.stdout:
		current_data = line.decode ('utf-8').strip ()

		if len(current_data) == 0:
			continue

		current_type = current_data[0]
		current_data = current_data[1:]

		# TODO: Check that input has correct format

		if current_data[0] == '[' and current_data[-1] == ']':
			current_data = current_data[1:-1]

		if current_type == 'w':
			if len(current_data) == 1:
				write.putchar (current_data)
			elif current_data == '\\n':
				write.newline ()
		elif current_type == 'm':
			pass
