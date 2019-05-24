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
		print (line)
		current_data = current_data.split (' ')

		print (current_data)

		if len(current_data) == 0:
			continue

		if current_data[0] == 'w':
			if len(current_data) == 1:
				write.putchar (current_data[1])
			elif current_data == '\\n':
				write.newline ()
		elif current_data[0] == 'm':
			pass
