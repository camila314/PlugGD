from base cimport *

cdef public class UIHandler [object UIHandler, type UIHandler]:
	@staticmethod
	def mouseCoords():
		thing = getMouseCoords()

		return (thing.x, thing.y,)