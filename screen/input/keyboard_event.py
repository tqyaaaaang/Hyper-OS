#!/usr/bin/env python3

"""
screen.input.keyboard_event
Handle keyboard events
"""

from screen import data
from screen.screen import utils
from screen import interact

from prompt_toolkit import document

def handle_keyboard_event (event):
	current_key = event.key_sequence[0].key
	current_data = event.key_sequence[0].data
	utils.move_cursor_to_end (data.messages_window.buffer)
	data.key_seq.append (current_key)
	if len(data.key_seq) > 100:
		data.key_seq.pop ( 0 )
	interact.write.write ('k', '{}'.format (current_key))
