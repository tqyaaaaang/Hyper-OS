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

	@data.key_binding.add('<any-visible>')
	@data.key_binding.add('escape')
	@data.key_binding.add('c-b')
	@data.key_binding.add('c-c')
	@data.key_binding.add('c-d')
	@data.key_binding.add('c-e')
	@data.key_binding.add('c-f')
	@data.key_binding.add('c-g')
	@data.key_binding.add('c-h')
	@data.key_binding.add('c-i')
	@data.key_binding.add('c-j')
	@data.key_binding.add('c-k')
	@data.key_binding.add('c-l')
	@data.key_binding.add('c-m')
	@data.key_binding.add('c-n')
	@data.key_binding.add('c-o')
	@data.key_binding.add('c-p')
	@data.key_binding.add('c-q')
	@data.key_binding.add('c-r')
	@data.key_binding.add('c-s')
	@data.key_binding.add('c-t')
	@data.key_binding.add('c-u')
	@data.key_binding.add('c-v')
	@data.key_binding.add('c-w')
	@data.key_binding.add('c-x')
	@data.key_binding.add('c-y')
	@data.key_binding.add('c-z')
	@data.key_binding.add('f1')
	@data.key_binding.add('f2')
	@data.key_binding.add('f3')
	@data.key_binding.add('f4')
	@data.key_binding.add('f5')
	@data.key_binding.add('f6')
	@data.key_binding.add('f7')
	@data.key_binding.add('f8')
	@data.key_binding.add('f9')
	@data.key_binding.add('f10')
	@data.key_binding.add('f11')
	@data.key_binding.add('f12')
	@data.key_binding.add('f13')
	@data.key_binding.add('f14')
	@data.key_binding.add('f15')
	@data.key_binding.add('f16')
	@data.key_binding.add('f17')
	@data.key_binding.add('f18')
	@data.key_binding.add('f19')
	@data.key_binding.add('f20')
	@data.key_binding.add('c-@')  # Also c-space.
	@data.key_binding.add('c-\\')
	@data.key_binding.add('c-]')
	@data.key_binding.add('c-^')
	@data.key_binding.add('c-_')
	@data.key_binding.add('backspace')
	@data.key_binding.add('up')
	@data.key_binding.add('down')
	@data.key_binding.add('right')
	@data.key_binding.add('left')
	@data.key_binding.add('s-up')
	@data.key_binding.add('s-down')
	@data.key_binding.add('s-right')
	@data.key_binding.add('s-left')
	@data.key_binding.add('home')
	@data.key_binding.add('end')
	@data.key_binding.add('delete')
	@data.key_binding.add('s-delete')
	@data.key_binding.add('c-delete')
	@data.key_binding.add('pageup')
	@data.key_binding.add('pagedown')
	@data.key_binding.add('s-tab')
	@data.key_binding.add('tab')
	@data.key_binding.add('c-left')
	@data.key_binding.add('c-right')
	@data.key_binding.add('c-up')
	@data.key_binding.add('c-down')
	@data.key_binding.add('insert')
	@data.key_binding.add(keys.Keys.Ignore)
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
