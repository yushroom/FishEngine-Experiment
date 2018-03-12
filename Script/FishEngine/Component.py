from . import Object
import FishEngineInternal
from typing import Type, List, Set
import sys

class Component(Object):
    __slots__ = ('__gameObject')
    def __init__(self):
        super().__init__()
        # self.__tag = ""

    # def __del__(self):
    #     print('Component.__del__')
    #     # self.m_gameObject = None

    @property
    def gameObject(self)->"GameObject":
        # cppGO = self.cpp.GetGameObject()
        # # print(cppGO.name, cppGO.__class__)
        # if cppGO.GetPyObject() is None:
        #     from . import GameObject
        #     return GameObject("", cppGO)
        # return None if cppGO is None else cppGO.GetPyObject()
        cppGO = self.cpp.GetGameObject()
        return None if cppGO is None else cppGO.GetPyObject()
    @gameObject.setter
    def gameObject(self, go:"GameObject"):
        from . import GameObject
        assert(self.gameObject is None)
        assert(isinstance(go, GameObject))
        go.AddComopnent(self)

    @property
    def transform(self)->'Transform':
        go = self.gameObject
        return None if go is None else go.transform

    @staticmethod
    def FindByType(componentType:Type('Component'))->List['Component']:
        return Object.FindObjectsOfType(componentType)

    @staticmethod
    def FindByTypes(*componentTypes)->Set['Component']:
        return set.intersection(*[Component.FindByType(t) for t in componentTypes])

    def Serialize(self, dumper):
        super(Component, self).Serialize(dumper)
        dumper.d('m_GameObject', self.gameObject)

    @staticmethod
    def WrapCPP(cppComp):
        assert(isinstance(cppComp, FishEngineInternal.Component))
        name = cppComp.__class__.__name__
        wrap = getattr(sys.modules['FishEngine'], name)()
        wrap.m_CachedPtr = cppComp
        cppComp.SetPyObject(wrap)