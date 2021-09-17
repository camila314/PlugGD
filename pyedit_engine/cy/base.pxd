#cython: language_level=3
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "extra_bindings.h":
    struct event_base:
        string name
    struct add_object_event:
        string name
        int object_id

    struct CCPoint:
        float x
        float y

    cppclass CCObject:
        void retain()
        void release()

    cppclass CCNode:
        void setPosition(CCPoint pt)
        CCPoint getPosition()
        void setRotation(float)
        float getRotation()
        void setScale(float)
        float getScale()
    cppclass CCArray(CCObject):
        @staticmethod
        CCArray* create()
        int count()
        CCObject* objectAtIndex(unsigned int)
        void addObject(CCObject*)

    EditorUI* EditorUI_shared()
    bool onMainThread()
    long makeUsable(void*)
    CCPoint getMouseCoords()
    char* getNode(CCObject*)
    event_base* popEvent()
    void clearEvents()
    vector[int] getObjGroups(GameObject*)

    cppclass LevelEditorLayer:
        GameObject* createObject(int, CCPoint, bool)
        CCArray* getAllObjects()

    cppclass EditorUI:
        void selectObjects(CCArray*, bool)
        void pasteObjects(string)
        void onDuplicate(CCObject*)
        void onDeleteSelected(CCObject*)
        CCArray* getSelectedObjects()
        void deselectAll()

    LevelEditorLayer* getEditorLayer(EditorUI* eui)

    struct GJSpriteColor:
        float hue
        float sat
        float bright

    cppclass GameObject(CCNode):
        void destroyObject()
        string getSaveString()
        void updateCustomScale(float)
        @staticmethod
        GameObject* createWithKey(int k)
    
    GJSpriteColor* getRelSpriteColor(GameObject* g, int k)

    int& gob_getZOrder(GameObject*)
    int gob_getID(GameObject*)

    cppclass ThreadController:
        @staticmethod
        ThreadController* sharedState()
        bool schedulePy(object p)

cdef inline mainThread(object o):
    ThreadController.sharedState().schedulePy(o)