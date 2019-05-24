#!/usr/bin/env python3

"""
screen.input.keyboard_event
Handle keyboard events
"""

from screen import data

def handle_keyboard_event (event):
	current_key = event.key_sequence[0].key
	current_data = event.key_sequence[0].data
	data.messages_window.buffer.insert_text (repr(event) + ' ' + current_key + ' ' + current_data + '\n')
	data.key_seq.append (current_key)
	if len(data.key_seq) > 100:
		data.key_seq.pop ( 0 )
	print (current_key + ' ' + current_data)
