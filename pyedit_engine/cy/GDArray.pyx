#cython: language_level=3
from base cimport *

cdef public class PyCCArray(PyCCObject) [object _CCArray, type __CCArray]:

    cdef CCArray* arr_inst(self):
        if  <long>self.inst == 0L:
            raise ValueError("Array got deallocated")
        return <CCArray*>self.inst

    def __len__(self):
        return thread_sync(lambda: self.arr_inst().count())()

    @thread_sync
    def __getitem__(self, e):
        if type(e) != int:
            raise ValueError("Invalid index")
        if e >= len(self):
            raise IndexError("CCArray index out of range")
        return PyCCObject().fromPtr(self.arr_inst().objectAtIndex(e)).reinterpret_cast()
    def sortBy(self, toSort):
        return list(sorted(self, key=toSort))
    def collect(self):
        return [x for x in self]

    def init(self, stuff=[]):
        if not issubclass(type(stuff), PyCCArray) and type(stuff) != list:
            raise ValueError("Input must be a list or subclass of PyCCArray")
        
        if type(stuff) != list:
            stuff = stuff.collect()

        cdef CCArray* ptr = CCArray.create()
        ptr.retain()

        cdef PyCCObject obj
        for obj in stuff:
            if not issubclass(type(obj), PyCCObject):
                raise ValueError(f"Invalid type '{obj.__class__.__name__}' for item inside list")
            ptr.addObject(obj.inst)
        self.inst = <CCObject*>ptr
        return self



cdef public class GameObjArray(PyCCArray) [object _GOArray, type __GOArray]:
    @thread_sync
    def __getattr__(self, attr):
        stuff = [getattr(self[x], attr) for x in range(len(self))]
        if stuff[0].__class__.__name__ != "builtin_function_or_method":
            print(stuff[0])
            raise AttributeError(f"'GameObjArray' object has no attribute '{attr}'")
        return lambda *args: [s(*args) for s in stuff]
    def sortX(self):
        return sorted(self, key=lambda x: x.positionX)
