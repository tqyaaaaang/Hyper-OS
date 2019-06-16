#!/usr/bin/env python3

"""
screen.__init__
"""


from prompt_toolkit import eventloop

from screen import data


def screen_exit ():
	def application_exit ():
		data.application.exit ()
		data.screen_down_event.set ()
	eventloop.get_event_loop ().call_from_executor (application_exit)
	data.screen_down_event.wait ()
	while data.application.is_running:
		pass
