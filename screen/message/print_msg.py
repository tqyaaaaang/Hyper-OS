#!/usr/bin/env python3

"""
screen.message.print_msg
Print messages
"""


from screen import data
from screen.screen import utils


def print_message (msg):
	utils.move_cursor_to_end (data.messages_window.buffer)
	data.messages_window.buffer.insert_text (msg)
	data.messages_window.buffer.newline ()
