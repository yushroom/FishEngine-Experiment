from . import Object
import FishEngineInternal
from typing import Type, List, Set
import sys

class __Component:
    @property
    def gameObject(self)->"GameObject":
        return self.GetGameObject()
    @gameObject.setter
    def gameObject(self, go:"GameObject"):
        from . import GameObject
        assert(self.gameObject is None)
        assert(isinstance(go, GameObject))
        go.AddComopnent(self)

    @property
    def transform(self)->'Transform':
        return self.gameObject.transform

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
        super(Component, self).Deserialize(loader)
        self.gameObject = loader['m_GameObject']

# def Component__new__(cls):
#     return FishEngineInternal.CreateComponent()

# def Component__init__(self):
#     pass

Component = FishEngineInternal.Component
# Component.__new__ = Component__new__
# Component.__init__ = Component__init__
# Component.ClassID = FishEngineInternal.ComponentClassID()
Component.gameObject = __Component.gameObject
Component.transform = __Component.transform
Component.Serialize = __Component.Serialize