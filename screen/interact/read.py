#!/usr/bin/env python3

"""
screen.interact.read
Read service to read from kernel
"""


import threading
import os

import json

from screen import data
from screen.output import write
from screen import message
from screen import interact


read_thr = None


def init ():
	read_thr = threading.Thread (target=read_thread_entry)
	read_thr.start ()


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

		count_brac = 0
		config_split_i = 0
		for i in current_data:
			if i == '{':
				count_brac += 1
			elif i == '}':
				count_brac -= 1
			if count_brac == 0:
				break
			config_split_i += 1

		current_config = current_data[:config_split_i]
		current_data = current_data[config_split_i:]

		if current_config == '':
			current_config = {}
		else:
			current_config = json.loads (current_config)

		# TODO: Check that input has correct format

		if current_data[0] == '[' and current_data[-1] == ']':
			current_data = current_data[1:-1]

		if current_type == 'w':
			if len(current_data) == 1:
				write.putchar (current_data)
			elif current_data == '\\n':
				write.newline ()
		elif current_type == 'm':
			message.print_message (current_data)
		elif current_type == 'c':
			if current_data == 'backspace':
				write.backspace ()
			elif current_data == 'newline':
				write.newline ()
			elif current_data == 'cursor_move_left':
				write.cursor_move_left ()
			elif current_data == 'cursor_move_right':
				write.cursor_move_right ()
		elif current_type == 's':
			if current_data == 'shutdown':
				interact.write.write ('s', 'shutdown')
				break
