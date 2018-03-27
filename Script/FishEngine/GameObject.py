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

class __GameObject(Object):

    # __slots__ = ('m_PrefabInternal')
    # ClassID = FishEngineInternal.GameObject.ClassID

    # CreateWithTransform
    def __init__(self, name='GameObject', *, cppObject=None):
        from . import Transform, Prefab
        super().__init__()
        if cppObject is None:
            self.m_CachedPtr = FishEngineInternal.CreateGameObject()
            self.m_CachedPtr.name = name
        else:
            assert(isinstance(cppObject, FishEngineInternal.GameObject))
            self.m_CachedPtr = cppObject
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
        return self.GetTransform()
        # return self.m_CachedPtr.GetTransform().GetPyObject()

    @property
    def scene(self):
        return self.__scene

    @property
    def activeSelf(self) -> bool:
        '''The local active state of this GameObject. (Read Only) \n
        This returns the local active state of this GameObject, which is set using GameObject.SetActive. Note that a GameObject may be inactive because a parent is not active, even if this returns true. This state will then be used once all parents are active. Use GameObject.activeInHierarchy if you want to check if the GameObject is actually treated as active in the scene.
        '''
        return self.IsActive()
    
    @property
    def activeInHierarchy(self) -> bool:
        '''Is the GameObject active in the scene? \n
        This lets you know if a gameObject is active in the game. That is the case if its GameObject.activeSelf property is enabled, as well as that of all it's parents.
        '''
        return self.cpp.IsActiveInHierarchy()

    def SetActive(self, active: bool):
        '''Activates/Deactivates the GameObject.
        '''
        self.cpp.SetActive(active)

    @property
    def scene2(self):
        return self.m_CachedPtr.GetScene()

    @property
    def components(self):
        return FishEngineInternal.GameObject_GetAllComponents(self)

    def GetComponent(self, type):
        from . import Component
        assert(issubclass(type, Component))
        return FishEngineInternal.GameObject_GetComopnent(self.m_CachedPtr, type.ClassID)

    def AddComponent(self, component:'Component')->'Component':
        from . import Component
        if isinstance(component, FishEngineInternal.Component):
            self.m_CachedPtr.AddComponent(component)
            return component
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
        
    def Serialize(self, dumper):
        super(GameObject, self).Serialize(dumper)
        # dumper.d('m_Component', self.components)
        dumper.d('m_Component', [{'component': c} for c in self.components ])
        dumper.d('m_Name', self.name)
        dumper.d('m_IsActive', self.activeSelf)

    def Deserialize(self, loader):
        super().Deserialize(loader)
        for comp in loader.d['m_Component']:
            self.AddComponent(comp)
        self.name = loader.d['m_Name']
        self.SetActive(loader.d['m_IsActive'])

    def Clone(self)->'GameObject':
        from . import Component
        cloned = GameObject(self.name)
        for c in FishEngineInternal.GameObject_GetAllComponents(cloned.cpp):
            Component.WrapCPP(c)
        for child in cloned.transform.children:
            pass
        return cloned


def GameObject__new__(cls, name):
    return FishEngineInternal.CreateGameObject()

def GameObject__init__(self, name="GameObject"):
    self.name = name

GameObject = FishEngineInternal.GameObject
GameObject.__new__ = GameObject__new__
GameObject.__init__ = GameObject__init__
GameObject.ClassID = FishEngineInternal.GameObjectClassID()
# GameObject.transform = __GameObject.transform
GameObject.Serialize = __GameObject.Serialize
GameObject.Deserialize = __GameObject.Deserialize
GameObject.components = __GameObject.components
GameObject.activeSelf = __GameObject.activeSelf