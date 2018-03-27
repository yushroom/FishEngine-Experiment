from . import Object
import FishEngineInternal
from typing import Type, List, Set
import sys

class __Component:
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

    def Deserialize(self, loader):
        super().Deserialize(loader)
        self.gameObject = loader['m_GameObject']

    @staticmethod
    def WrapCPP(cppComp):
        assert(isinstance(cppComp, FishEngineInternal.Component))
        name = cppComp.__class__.__name__
        wrap = getattr(sys.modules['FishEngine'], name)()
        wrap.m_CachedPtr = cppComp
        cppComp.SetPyObject(wrap)

def Component__new__(cls):
    return FishEngineInternal.CreateComponent()

def Component__init__(self):
    pass

Component = FishEngineInternal.Component
Component.__new__ = Component__new__
Component.__init__ = Component__init__
# Component.ClassID = FishEngineInternal.ComponentClassID()
Component.gameObject = __Component.gameObject