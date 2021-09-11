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

	def _runLoop(self):
		v = popEvent()
		if <long>v != 0:
			print("ev", v.name)
			for e in self.events:
				if e.handler == v.name.decode():
					print(e)
					e.callback(self.extractEv(v))

	def runLoop(self):
		clearEvents()
		exec("""while True:\n    self._runLoop()""", {**globals(), "self":self}) # python is gross

	def registerEvent(self, callback, handler, name):
		if handler in self.valid_events:
			self.events.append(PyEvent(handler, name, callback))
		else:
			import pygd
			pygd.alert("Error", "Invalid event handler {}".format(handler), "Ok")
			raise ValueError("no")

	def get_events(self):
		return self.events
