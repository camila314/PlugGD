#cython: language_level=3
#distutils: language = c++
from base cimport *

cdef public class Editor(PyCCObject) [object PyEditorUI, type PyEditUI]:
    """
    Handle to the EditorUI instance in gd. Gives you access to endpoints of things you would be able to do in the editor, 
    like select objects, duplicate the selection, create objects, and more.
    """
    cdef EditorUI* edit_inst(self):
        if makeUsable(self.inst) == 0:
            raise RuntimeError("Unable to fetch editor")
        return <EditorUI*>self.inst

    ## Gets a handle to the EditorUI instance if it exists. If it doesn't, an error is raised.
    def __init__(self):
        self.inst = <CCObject*>EditorUI_shared()
        if makeUsable(self.inst) == 0:
            raise RuntimeError("Unable to fetch editor")

    ## Pastes a level string into the game and selects it. 
    #  @param objs A level string
    @thread_async
    def pasteStr(self, objs):
        if type(objs)==str:
            objs = objs.encode()
        self.edit_inst().pasteObjects(objs)

    ## Creates an object. Does not select it.
    #  @param objectid The object ID
    #  @param x x position of object
    #  @param y y position of object
    @thread_sync
    def createObject(self, objectid, x=0.0, y=0.0):
        """

        """
        objectid = int(objectid)
        cdef CCPoint p
        p.x = float(x)
        p.y = float(y)+90.0

        cdef GameObject* o = getEditorLayer(self.edit_inst()).createObject(objectid, p, True)
        pyo = PyGameObject().fromPtr(<CCObject*>o)
        return pyo

    ## Getter for the selection. Usage: 
    #  @code 
    ## amount_selected = len(Editor().selection)
    #  @endcode
    @property
    def selection(self):
        c = thread_sync(lambda:GameObjArray().fromPtr(self.edit_inst().getSelectedObjects()))()
        return c

    ## Setter for the selection. Usage: 
    #  @code 
    ## Editor().selection = Editor().objects
    #  @endcode
    @selection.setter
    def selection(self, objects):
        cdef GameObjArray ccar = GameObjArray().init(objects)
        thread_sync(lambda: self.edit_inst().selectObjects(ccar.arr_inst(), False))()

    ## Getter for the objects in a level. Usage: 
    #  @code 
    ## Editor().select(Editor().objects[)
    #  @endcode
    @property
    def objects(self):
        c = thread_sync(lambda:GameObjArray().fromPtr(getEditorLayer(self.edit_inst()).getAllObjects()))()
        return c

    ## Deselects all objects
    def deselect(self):
        self.edit_inst().deselectAll()

    ## Deletes everything selected
    @thread_sync
    def deleteSelection(self):
        self.edit_inst().onDeleteSelected(<CCObject*>self.edit_inst())

    ## Duplicates the selection
    @thread_sync
    def duplicate(self):
        self.edit_inst().onDuplicate(<CCObject*>self.edit_inst())
        return self.selection

    ## Sets the selection
    #  @param object_s List of new objects to select
    def select(self, object_s):
        if issubclass(type(object_s), PyGameObject):
            object_s = [object_s]
        cdef GameObjArray ccar = GameObjArray().init(object_s)
        self.edit_inst().selectObjects(ccar.arr_inst(), True)