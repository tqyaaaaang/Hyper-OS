#!/usr/bin/env python3

"""
screen.output.write
Write to screen
"""


from screen import data


def putchar (ch):
	data.console_window.buffer.insert_text (ch)

def newline ():
	data.console_window.buffer.newline ()

def backspace ():
	data.console_window.buffer.delete_before_cursor ()
