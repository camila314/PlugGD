#cython: language_level=3
#distutils: language = c++
from base cimport *

cdef public class PyGameObject(PyCCObject) [object PyGameOb, type PyGameObj]:
    """
    Represents an in-game object
    """
    
    cdef GameObject* gameob_inst(self):
        return <GameObject*>self.inst

    def moveTo(self, x, y):
        cdef CCPoint p
        p.x = x
        p.y = y+90
        self.gameob_inst().setPosition(p)
        return self
    def move(self, x, y):
        cdef CCPoint p = self.gameob_inst().getPosition()
        p.x += x
        p.y += y
        self.gameob_inst().setPosition(p)
        return self

    @property
    def base_hue(self):
        if <long>getRelSpriteColor(self.gameob_inst(), 0) != 0:
            return getRelSpriteColor(self.gameob_inst(), 0).hue
        else:
            return 0.0

    @base_hue.setter
    def base_hue(self, v):
        if <long>getRelSpriteColor(self.gameob_inst(), 0) != 0:
            getRelSpriteColor(self.gameob_inst(), 0).hue = v
        else:
            pass

    @property
    def detail_hue(self):
        if <long>getRelSpriteColor(self.gameob_inst(), 2) != 0:
            return getRelSpriteColor(self.gameob_inst(), 2).hue
        else:
            return 0.0

    @detail_hue.setter
    def detail_hue(self, v):
        if <long>getRelSpriteColor(self.gameob_inst(), 2) != 0:
            getRelSpriteColor(self.gameob_inst(), 2).hue = v
        else:
            pass

    def incrementZ(self):
        (&gob_getZOrder(self.gameob_inst()))[0] = gob_getZOrder(self.gameob_inst()) + 1

    @property
    def size(self):
        return self.gameob_inst().getScale()
    def scaleTo(self, sc):
        self.gameob_inst().updateCustomScale(sc)
        return self
    def scale(self, sc):
        return self.scaleTo(self.size + sc)

    @property
    def id(self):
        return gob_getID(self.gameob_inst())

    @property
    def position(self):
        cdef CCPoint p = self.gameob_inst().getPosition()
        return (p.x, p.y-90)
    @property
    def positionX(self):
        cdef CCPoint p = self.gameob_inst().getPosition()
        return p.x
    @property
    def positionY(self):
        cdef CCPoint p = self.gameob_inst().getPosition()
        return p.y-90
    @property
    def levelStr(self):
        return self.gameob_inst().getSaveString()

    @property
    def groups(self):
        return getObjGroups(self.gameob_inst())

    def rotate(self, deg):
        return self.rotateTo(self.rotation + deg)
    def rotateTo(self, deg):
        self.gameob_inst().setRotation(deg)
        return self
    @property
    def rotation(self):
        return self.gameob_inst().getRotation()

include "ObjectExtensions.pyx"