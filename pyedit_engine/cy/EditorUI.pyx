#cython: language_level=3
#distutils: language = c++
from base cimport *

cdef public class Editor(PyCCObject) [object PyEditorUI, type PyEditUI]:

    cdef EditorUI* edit_inst(self):
        if makeUsable(self.inst) == 0:
            raise RuntimeError("Unable to fetch editor")
        return <EditorUI*>self.inst

    def __init__(self):
        self.inst = <CCObject*>EditorUI_shared()
        if makeUsable(self.inst) == 0:
            raise RuntimeError("Unable to fetch editor")
    @thread_async
    def pasteStr(self, obs):
        if type(obs)==str:
            obs = obs.encode()
        self.edit_inst().pasteObjects(obs)

    @thread_sync
    def createObject(self, objectid, x=0.0, y=0.0):
        objectid = int(objectid)
        cdef CCPoint p
        p.x = float(x)
        p.y = float(y)+90.0

        cdef GameObject* o = getEditorLayer(self.edit_inst()).createObject(objectid, p, True)
        pyo = PyGameObject().fromPtr(<CCObject*>o)
        return pyo

    @property
    def selection(self):
        c = thread_sync(lambda:GameObjArray().fromPtr(self.edit_inst().getSelectedObjects()))()
        return c

    @property
    def objects(self):
        c = thread_sync(lambda:GameObjArray().fromPtr(getEditorLayer(self.edit_inst()).getAllObjects()))()
        return c

    def deselect(self):
        self.edit_inst().deselectAll()

    @thread_sync
    def deleteSelection(self):
        self.edit_inst().onDeleteSelected(<CCObject*>self.edit_inst())

    @thread_sync
    def duplicate(self):
        self.edit_inst().onDuplicate(<CCObject*>self.edit_inst())
        return self.selection

    @selection.setter
    def selection(self, objects):
        cdef GameObjArray ccar = GameObjArray().init(objects)
        thread_sync(lambda: self.edit_inst().selectObjects(ccar.arr_inst(), False))()

    def select(self, object_s):
        if issubclass(type(object_s), PyGameObject):
            object_s = [object_s]
        cdef GameObjArray ccar = GameObjArray().init(object_s)
        self.edit_inst().selectObjects(ccar.arr_inst(), True)