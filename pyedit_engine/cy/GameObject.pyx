#cython: language_level=3
#distutils: language = c++
from base cimport *

cdef public class PyGameObject(PyCCObject) [object PyGameOb, type PyGameObj]:
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

    def incrementZ(self):
        (&self.gameob_inst()._zOrder())[0] = self.gameob_inst()._zOrder() + 1

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
        return self.gameob_inst()._id()

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
    def rotate(self, deg):
        return self.rotateTo(self.rotation + deg)
    def rotateTo(self, deg):
        self.gameob_inst().setRotation(deg)
        return self
    @property
    def rotation(self):
        return self.gameob_inst().getRotation()
