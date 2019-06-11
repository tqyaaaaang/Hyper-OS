#!/usr/bin/env python3

"""
screen.screen.utils
"""


import prompt_toolkit
from prompt_toolkit import document


def move_cursor_to_end (buffer):
	doc = buffer.document
	buffer.set_document (document.Document (doc.text, doc.cursor_position + doc.get_end_of_document_position ()))

def move_cursor_to_line_start (buffer):
	doc = buffer.document
	buffer.set_document (document.Document (doc.text, doc.cursor_position + doc.get_start_of_line_position ()))
