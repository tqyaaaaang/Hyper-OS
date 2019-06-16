#!/usr/bin/env python3

"""
screen.screen.__init__
Process the screen
"""

from . import app
from . import windows
from . import key_bindings

def init ():
	key_bindings.init ()
	windows.init ()
	app.init ()

def run ():
	app.run ()
