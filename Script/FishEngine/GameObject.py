from . import Object, Scene, accepts, returns, Debug
# from . import FishEngineInternal
import FishEngineInternal

import weakref
from typing import Type, Set
from enum import Enum, auto

class PrimitiveType(Enum):
    Sphere = auto()
    Capsule = auto()
    Cylinder = auto()
    Cube = auto()
    Plane = auto()
    Quad = auto()

class GameObject(Object):

    __slots__ = ('__transform', '__components', '__scene', 'm_IsActive', 'm_PrefabInternal')
    ClassID = FishEngineInternal.GameObject.ClassID

    # CreateWithTransform
    def __init__(self, name='GameObject'):
        from . import Transform, Prefab
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateGameObject()
        self.m_CachedPtr.name = name
        transform = Transform(self.m_CachedPtr.GetTransform())
        self.m_IsActive = True
        self.m_PrefabInternal:Prefab = None
        self.m_CachedPtr.SetPyObject(self)

    # @staticmethod
    # def CreateWithTransform(name:str="GameObject"):
    #     return GameObject(name)

    @staticmethod
    def CreateWithRectTransform(name:str="GameObject"):
        from . import RectTransform
        go = GameObject(name)
        go.AddComponent(RectTransform())
        return go

    # def __del__(self):
    #     print("GameObject.__del__")
    #     # Object.DestroyImmediate(self.__transform)

    def Clean(self):
        # print('GameObject.Clean()')
        if self.m_PrefabInternal is not None:
            # self.m_PrefabInternal.m_RootGameObject = None
            self.m_PrefabInternal.Clean()

    @property
    def transform(self)->'Transform':
        # return self.__transform
        return self.m_CachedPtr.GetTransform().GetPyObject()

    @property
    def scene(self):
        return self.__scene

    @property
    def scene2(self):
        return self.m_CachedPtr.GetScene()

    # @property
    # def components(self):
    #     return self.__components

    def GetComponent(self, type):
        from . import Component
        assert(issubclass(type, Component))
        return FishEngineInternal.GameObject_GetComopnent(self.m_CachedPtr, type.ClassID)

    def AddComponent(self, component:'Component')->'Component':
        from . import Component
        assert(isinstance(component, Component))
        self.m_CachedPtr.AddComponent(component.m_CachedPtr)
        return component

    # ret GameObject
    # @accepts(PrimitiveType)
    # @returns(GameObject)
    @staticmethod
    def CreatePrimitive(type: PrimitiveType)->'GameObject':
        from . import MeshFilter, Mesh, MeshRenderer, Material
        go = GameObject(type.name)
        mf = go.AddComponent(MeshFilter())
        mf.mesh = Mesh.GetInternalMesh(type.name)
        mr = go.AddComponent(MeshRenderer())
        mr.material = Material.defaultMaterial()
        return go

    @staticmethod
    def FindWithComponent(componentType:Type('Component'))->Set['Component']:
        return {o.gameObject for o in Object.FindObjectsOfType(componentType)}

    @staticmethod
    def FindWithComponents(*componentTypes)->Set['Component']:
        return set.intersection(*[GameObject.FindWithComponent(t) for t in componentTypes])

    @staticmethod
    def Clone(go:'GameObject'):
        cloned = GameObject(go.name)
        cloned.__transform.localPosition = go.__transform.localPosition
        cloned.__transform.localRotation = go.__transform.localRotation
        cloned.__transform.local
        