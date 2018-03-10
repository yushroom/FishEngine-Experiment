from . import Object
import FishEngineInternal
from typing import Type, List, Set

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
        return self.m_CachedPtr.GetGameObject().GetPyObject()
    @gameObject.setter
    def gameObject(self, go:"GameObject"):
        assert(self.gameObject is None)
        from . import GameObject
        assert(isinstance(go, GameObject))
        go.AddComopnent(self)

    @property
    def transform(self):
        go = self.gameObject
        if go is None:
            return None
        return go.transform

    @staticmethod
    def FindByType(componentType:Type('Component'))->List['Component']:
        return Object.FindObjectsOfType(componentType)

    @staticmethod
    def FindByTypes(*componentTypes)->Set['Component']:
        return set.intersection(*[Component.FindByType(t) for t in componentTypes])