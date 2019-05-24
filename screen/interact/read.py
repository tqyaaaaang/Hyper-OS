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
	for current_data in data.kern_proc.stdout:
		current_data = current_data.decode ('utf-8').strip ()
		write.putchar (current_data)
