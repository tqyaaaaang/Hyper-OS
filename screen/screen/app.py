#!/usr/bin/env python3

"""
screen.app
Application
"""


import prompt_toolkit
from prompt_toolkit import layout
from prompt_toolkit.layout import containers
from prompt_toolkit import widgets
from prompt_toolkit.key_binding import key_processor

from screen import data
from screen.screen import key_bindings


def init ():
	root_container = containers.VSplit (data.app_windows)

	app_layout = layout.Layout (root_container)

	data.application = prompt_toolkit.Application (layout=app_layout, full_screen=True, mouse_support=True, key_bindings=data.key_binding)

def run ():
	data.application.run ()
