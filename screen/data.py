#!/usr/bin/env python3

"""
screen.screen.data
Data in screen application
"""


import threading

import prompt_toolkit
from prompt_toolkit import widgets
from prompt_toolkit.key_binding import KeyBindings


application = prompt_toolkit.Application ()

console_window = widgets.TextArea ()
messages_window = widgets.TextArea ()
app_windows = []

screen_up_event = threading.Event ()
screen_down_event = threading.Event ()

key_binding = KeyBindings ()

key_seq = []

kern_proc = None

output_lock = threading.Lock ()
