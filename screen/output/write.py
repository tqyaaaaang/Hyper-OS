#!/usr/bin/env python3

"""
screen.output.write
Write to screen
"""


from screen import data
from screen.screen import utils


def putchar (ch):
	data.console_window.buffer.insert_text (ch)

def newline ():
	data.console_window.buffer.newline ()
	utils.move_cursor_to_line_start (data.console_window.buffer)

def backspace ():
	data.console_window.buffer.delete_before_cursor ()

def cursor_move_left ():
	data.console_window.buffer.cursor_left ()

def cursor_move_right ():
	data.console_window.buffer.cursor_right ()
