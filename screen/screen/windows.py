#!/usr/bin/env python3

"""
screen.screen.windows
Window objects
"""


import prompt_toolkit
from prompt_toolkit import layout
from prompt_toolkit.layout import containers
from prompt_toolkit import widgets

from screen import data


def init ():
	data.console_window = widgets.TextArea (focus_on_click=True)
	data.messages_window = widgets.TextArea (scrollbar=True, focus_on_click=True)

	data.app_windows = [
		widgets.Frame (data.console_window, title='console'),
		widgets.Frame (data.messages_window, title='messages')
	]
