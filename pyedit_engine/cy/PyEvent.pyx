from base cimport *
from collections import namedtuple
import threading



PyEvent = namedtuple('PyEvent', 'handler name callback')
cdef class PyEventHandler:
	cdef list events;
	cdef list valid_events;

	cdef extractEv(self, event_base* ev):
		if ev.name == b"add_object":
			the = <add_object_event*>ev
			obj = Editor().selection[0]
			return {"name": "add_object", "object": obj}
		else:
			return {"name": ev.name}

	def __cinit__(self):
		self.events = []
		self.valid_events = [
			"add_object", "remove_objects", "move_objects", "press_edit_button",
			"press_menu_button", "select_objects", "press_key"
		]

	def runLoop(self):
		while True:
			v = popEvent()
			if <long>v != 0:
				print("ev", v.name)
				for e in self.events:
					if e.handler == v.name.decode():
						print(e)
						e.callback(self.extractEv(v))
			else:
				exec("globals()")

	def registerEvent(self, callback, handler, name):
		self.events.append(PyEvent(handler, name, callback))

	def get_events(self):
		return self.events
