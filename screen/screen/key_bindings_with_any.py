#!/usr/bin/env python3

"""
screen.key_bindings_with_any
Class KeyBindingsWithAny
"""


from typing import (
	TYPE_CHECKING,
	Callable,
	Hashable,
	List,
	Optional,
	Sequence,
	Tuple,
	TypeVar,
	Union,
	cast,
)
from six import text_type

import prompt_toolkit
from prompt_toolkit import key_binding
from prompt_toolkit.key_binding.key_bindings import _Binding
from prompt_toolkit.filters import to_filter, Never
from prompt_toolkit.keys import Keys, ALL_KEYS, KEY_ALIASES
from prompt_toolkit.cache import SimpleCache
from prompt_toolkit.filters import to_filter, Never

from screen import data


KeysTuple = Tuple[Union[Keys, str], ...]


class KeyBindingsWithAny (key_binding.KeyBindingsBase):
	"""
	A container for a set of key bindings.

	Example usage::

		kb = KeyBindings()

		@kb.add('c-t')
		def _(event):
			print('Control-T pressed')

		@kb.add('c-a', 'c-b')
		def _(event):
			print('Control-A pressed, followed by Control-B')

		@kb.add('c-x', filter=is_searching)
		def _(event):
			print('Control-X pressed')  # Works only if we are searching.

	"""
	def __init__(self):
		self.bindings = []
		self._get_bindings_for_keys_cache = SimpleCache(maxsize=10000)
		self._get_bindings_starting_with_keys_cache = SimpleCache(maxsize=1000)
		self.__version = 0  # For cache invalidation.

	def _clear_cache(self):
		self.__version += 1
		self._get_bindings_for_keys_cache.clear()
		self._get_bindings_starting_with_keys_cache.clear()

	@property
	def _version(self):
		return self.__version

	def add(self, *keys, **kwargs):
		"""
		Decorator for adding a key bindings.

		:param filter: :class:`~prompt_toolkit.filters.Filter` to determine
			when this key binding is active.
		:param eager: :class:`~prompt_toolkit.filters.Filter` or `bool`.
			When True, ignore potential longer matches when this key binding is
			hit. E.g. when there is an active eager key binding for Ctrl-X,
			execute the handler immediately and ignore the key binding for
			Ctrl-X Ctrl-E of which it is a prefix.
		:param is_global: When this key bindings is added to a `Container` or
			`Control`, make it a global (always active) binding.
		:param save_before: Callable that takes an `Event` and returns True if
			we should save the current buffer, before handling the event.
			(That's the default.)
		:param record_in_macro: Record these key bindings when a macro is
			being recorded. (True by default.)
		"""
		filter = to_filter(kwargs.pop('filter', True))
		eager = to_filter(kwargs.pop('eager', False))
		is_global = to_filter(kwargs.pop('is_global', False))
		save_before = kwargs.pop('save_before', lambda e: True)
		record_in_macro = to_filter(kwargs.pop('record_in_macro', True))

		assert not kwargs
		assert keys
		assert callable(save_before)

		keys = tuple(_check_and_expand_key(k) for k in keys)

		if isinstance(filter, Never):
			# When a filter is Never, it will always stay disabled, so in that
			# case don't bother putting it in the key bindings. It will slow
			# down every key press otherwise.
			def decorator(func):
				return func
		else:
			def decorator(func):
				if isinstance(func, _Binding):
					# We're adding an existing _Binding object.
					self.bindings.append(
						_Binding(
							keys, func.handler,
							filter=func.filter & filter,
							eager=eager | func.eager,
							is_global = is_global | func.is_global,
							save_before=func.save_before,
							record_in_macro=func.record_in_macro))
				else:
					self.bindings.append(
						_Binding(keys, func, filter=filter, eager=eager,
								 is_global=is_global, save_before=save_before,
								 record_in_macro=record_in_macro))
				self._clear_cache()

				return func
		return decorator

	def remove(self, *args):
		"""
		Remove a key binding.

		This expects either a function that was given to `add` method as
		parameter or a sequence of key bindings.

		Raises `ValueError` when no bindings was found.

		Usage::

			remove(handler)  # Pass handler.
			remove('c-x', 'c-a')  # Or pass the key bindings.
		"""
		found = False

		if callable(args[0]):
			assert len(args) == 1
			function = args[0]

			# Remove the given function.
			for b in self.bindings:
				if b.handler == function:
					self.bindings.remove(b)
					found = True

		else:
			assert len(args) > 0

			# Remove this sequence of key bindings.
			keys = tuple(_check_and_expand_key(k) for k in args)

			for b in self.bindings:
				if b.keys == keys:
					self.bindings.remove(b)
					found = True

		if found:
			self._clear_cache()
		else:
			# No key binding found for this function. Raise ValueError.
			raise ValueError('Binding not found: %r' % (function, ))

	# For backwards-compatibility.
	add_binding = add
	remove_binding = remove

	def get_bindings_for_keys(self, keys):
		"""
		Return a list of key bindings that can handle this key.
		(This return also inactive bindings, so the `filter` still has to be
		called, for checking it.)

		:param keys: tuple of keys.
		"""
		def get():
			result = []
			for b in self.bindings:
				if len(keys) == len(b.keys):
					match = True
					any_count = 0

					for i, j in zip(b.keys, keys):
						if i != j and (i != Keys.Any and (i != '<any-visible>' or (j in ALL_KEYS))):
							match = False
							break

						if i == Keys.Any:
							any_count += 1

					if match:
						result.append((any_count, b))

			# Place bindings that have more 'Any' occurrences in them at the end.
			result = sorted(result, key=lambda item: -item[0])

			return [item[1] for item in result]

		return self._get_bindings_for_keys_cache.get(keys, get)

	def get_bindings_starting_with_keys(self, keys):
		"""
		Return a list of key bindings that handle a key sequence starting with
		`keys`. (It does only return bindings for which the sequences are
		longer than `keys`. And like `get_bindings_for_keys`, it also includes
		inactive bindings.)

		:param keys: tuple of keys.
		"""
		def get():
			result = []
			for b in self.bindings:
				if len(keys) < len(b.keys):
					match = True
					for i, j in zip(b.keys, keys):
						if i != j and (i != Keys.Any and (i != '<any-visible>' or (j in ALL_KEYS))):
							match = False
							break
					if match:
						result.append(b)
			return result

		return self._get_bindings_starting_with_keys_cache.get(keys, get)


def _check_and_expand_key(key):
	"""
	Replace key by alias and verify whether it's a valid one.
	"""
	# Lookup aliases.
	key = KEY_ALIASES.get(key, key)

	# Replace 'space' by ' '
	if key == 'space':
		key = ' '

	# Final validation.
	assert isinstance(key, text_type), 'Got %r' % (key, )
	if len(key) != 1 and key not in ALL_KEYS and key != '<any-visible>':
		raise ValueError('Invalid key: %s' % (key, ))

	return key



key_binding.KeyBindings.add = KeyBindingsWithAny.add
key_binding.KeyBindings.add_binding = KeyBindingsWithAny.add_binding
key_binding.KeyBindings.get_bindings_for_keys = KeyBindingsWithAny.get_bindings_for_keys
key_binding.KeyBindings.get_bindings_starting_with_keys = KeyBindingsWithAny.get_bindings_starting_with_keys
key_binding.KeyBindings.remove = KeyBindingsWithAny.remove
key_binding.KeyBindings.remove_binding = KeyBindingsWithAny.remove_binding
key_binding._check_and_expand_key = _check_and_expand_key
