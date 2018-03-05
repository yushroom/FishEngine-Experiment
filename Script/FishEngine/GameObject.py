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
        self.__components = []
        self.m_CachedPtr = FishEngineInternal.GameObject.Create(name)
        self.__transform = Transform()
        # if useRectTransform:
        #     self.m_CachedPtr = FishEngineInternal.GameObject.CreateWithTransform(name)
        #     self.__transform = Transform()
        # else:
        #     self.m_CachedPtr = FishEngineInternal.GameObject.CreateWithRectTransform(name)
        #     self.__transform = RectTransform()
        self.__transform.m_CachedPtr = self.m_CachedPtr.transform
        self.__transform.gameObject = self  # also add transform to __components
        self.__scene:Scene = Scene.SceneManager.GetActiveScene()
        self.__scene.gameObjects.append(self)
        self.m_IsActive = True
        self.m_PrefabInternal:Prefab = None
        self.m_CachedPtr.SetObject(self)
        self.__transform.m_CachedPtr.SetObject( self.m_CachedPtr.transform )

    # @staticmethod
    # def CreateWithTransform(name:str="GameObject"):
    #     return GameObject(name)

    @staticmethod
    def CreateWithRectTransform(name:str="GameObject"):
        from . import RectTransform
        go = GameObject(name)
        # go.__components.pop()   # pop transform
        go.AddComponent(RectTransform())
        return go

    # def __del__(self):
    #     print("GameObject.__del__")
    #     # Object.DestroyImmediate(self.__transform)

    def Clean(self):
        # print('GameObject.Clean()')
        # manually break ref cycle
        for comp in self.__components:
            comp.gameObject = None
        # import sys
        # print(sys.getrefcount(self.m_CachedPtr))
        # self.m_CachedPtr = None
        # self.__components.clear()
        # self.__scene.gameObjects.remove(self)
        # if self.transform.parent is None:
        #     self.__scene.m_rootGameObjects.remove(self)
        if self.m_PrefabInternal is not None:
            # self.m_PrefabInternal.m_RootGameObject = None
            self.m_PrefabInternal.Clean()
        self.__transform.Clean()

    @property
    def transform(self)->'Transform':
        return self.__transform

    @property
    def scene(self):
        return self.__scene

    @property
    def components(self):
        return self.__components

    def GetComponent(self, type):
        for comp in self.__components:
            if isinstance(comp, type):
                return comp
        return None

    # component: Component
    def AddComponent(self, component:'Component')->'Component':
        from . import Component
        assert(isinstance(component, Component))
        # if isinstance(component, RectTransform):
        #     old = self.__transform
        #     new = component
        #     if old.parent is not None:
        #         old.parent.children[old.m_RootOrder] = new
        #     new.m_RootOrder = old.m_RootOrder
        #     new.__parent = old.__parent
        #     old.parent[]
        #     new.m_CachedPtr = old.m_CachedPtr
        #     old.__parent =
        #     self.__transform = component
        component.gameObject = self
        assert(component.m_CachedPtr is not None)
        self.m_CachedPtr.AddComponent(component.m_CachedPtr)
        return component

    # ret GameObject
    # @accepts(PrimitiveType)
    # @returns(GameObject)
    @staticmethod
    def CreatePrimitive(type: PrimitiveType)->'GameObject':
        from . import MeshFilter, Mesh, MeshRenderer, Material
        go = GameObject("GameObject")
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