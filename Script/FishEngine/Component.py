from . import Object
import FishEngineInternal
import weakref

class Component(Object):
    __slots__ = ('__gameObject')
    def __init__(self):
        # self.name = "Transform"
        super().__init__()
        self.__gameObject = None    # __gameObject is a weakref
        # self.__tag = ""

    # def __del__(self):
    #     print('Component.__del__')
    #     # self.m_gameObject = None

    @property
    def gameObject(self)->"GameObject":
        if self.__gameObject is None:
            return None
        return self.__gameObject()
    @gameObject.setter
    def gameObject(self, go:"GameObject"):
        # from . import GameObject
        # assert(isinstance(go, GameObject))
        if self.__gameObject == go:
            return
        if go is not None:
            self.__gameObject = weakref.ref(go)
            go.components.append(self)
        else:
            go = None

    @property
    def transform(self):
        go = self.gameObject
        if go is None:
            return None
        return go.transform

# Component = FishEngineInternal.Component