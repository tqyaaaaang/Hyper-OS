#!/usr/bin/env python3

"""
screen.screen.key_bindings
Key bindings
"""


import prompt_toolkit
from prompt_toolkit import key_binding
from prompt_toolkit import keys

from screen import data
from screen.screen import key_bindings_with_any
from screen.input import keyboard_event


def init ():
	data.key_binding = key_bindings_with_any.KeyBindingsWithAny ()

	@data.key_binding.add('<any-visible>', eager=True)
	@data.key_binding.add('escape', eager=True)
	@data.key_binding.add('c-b', eager=True)
	@data.key_binding.add('c-c', eager=True)
	@data.key_binding.add('c-d', eager=True)
	@data.key_binding.add('c-e', eager=True)
	@data.key_binding.add('c-f', eager=True)
	@data.key_binding.add('c-g', eager=True)
	@data.key_binding.add('c-h', eager=True)
	@data.key_binding.add('c-i', eager=True)
	@data.key_binding.add('c-j', eager=True)
	@data.key_binding.add('c-k', eager=True)
	@data.key_binding.add('c-l', eager=True)
	@data.key_binding.add('c-m', eager=True)
	@data.key_binding.add('c-n', eager=True)
	@data.key_binding.add('c-o', eager=True)
	@data.key_binding.add('c-p', eager=True)
	@data.key_binding.add('c-q', eager=True)
	@data.key_binding.add('c-r', eager=True)
	@data.key_binding.add('c-s', eager=True)
	@data.key_binding.add('c-t', eager=True)
	@data.key_binding.add('c-u', eager=True)
	@data.key_binding.add('c-v', eager=True)
	@data.key_binding.add('c-w', eager=True)
	@data.key_binding.add('c-x', eager=True)
	@data.key_binding.add('c-y', eager=True)
	@data.key_binding.add('c-z', eager=True)
	@data.key_binding.add('f1', eager=True)
	@data.key_binding.add('f2', eager=True)
	@data.key_binding.add('f3', eager=True)
	@data.key_binding.add('f4', eager=True)
	@data.key_binding.add('f5', eager=True)
	@data.key_binding.add('f6', eager=True)
	@data.key_binding.add('f7', eager=True)
	@data.key_binding.add('f8', eager=True)
	@data.key_binding.add('f9', eager=True)
	@data.key_binding.add('f10', eager=True)
	@data.key_binding.add('f11', eager=True)
	@data.key_binding.add('f12', eager=True)
	@data.key_binding.add('f13', eager=True)
	@data.key_binding.add('f14', eager=True)
	@data.key_binding.add('f15', eager=True)
	@data.key_binding.add('f16', eager=True)
	@data.key_binding.add('f17', eager=True)
	@data.key_binding.add('f18', eager=True)
	@data.key_binding.add('f19', eager=True)
	@data.key_binding.add('f20', eager=True)
	@data.key_binding.add('c-@', eager=True)  # Also c-space.
	@data.key_binding.add('c-\\', eager=True)
	@data.key_binding.add('c-]', eager=True)
	@data.key_binding.add('c-^', eager=True)
	@data.key_binding.add('c-_', eager=True)
	@data.key_binding.add('backspace', eager=True)
	@data.key_binding.add('up', eager=True)
	@data.key_binding.add('down', eager=True)
	@data.key_binding.add('right', eager=True)
	@data.key_binding.add('left', eager=True)
	@data.key_binding.add('s-up', eager=True)
	@data.key_binding.add('s-down', eager=True)
	@data.key_binding.add('s-right', eager=True)
	@data.key_binding.add('s-left', eager=True)
	@data.key_binding.add('home', eager=True)
	@data.key_binding.add('end', eager=True)
	@data.key_binding.add('delete', eager=True)
	@data.key_binding.add('s-delete', eager=True)
	@data.key_binding.add('c-delete', eager=True)
	@data.key_binding.add('pageup', eager=True)
	@data.key_binding.add('pagedown', eager=True)
	@data.key_binding.add('s-tab', eager=True)
	@data.key_binding.add('tab', eager=True)
	@data.key_binding.add('c-left', eager=True)
	@data.key_binding.add('c-right', eager=True)
	@data.key_binding.add('c-up', eager=True)
	@data.key_binding.add('c-down', eager=True)
	@data.key_binding.add('insert', eager=True)
	@data.key_binding.add(keys.Keys.Ignore, eager=True)
	def process_any_key (event):
		keyboard_event.handle_keyboard_event (event)

	@data.key_binding.add ('c-a', 'escape')
	def process_exit (event):
		data.application.exit ()

	@data.key_binding.add ('c-a', 'c-a')
	def process_c_a (event):
		process_any_key (event)

	@data.key_binding.add ('c-a', '<any>')
	def process_c_a_null (event):
		pass
