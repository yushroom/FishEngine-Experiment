# from . import FishEngineInternal
# from . import Transform, GameObject
import FishEngineInternal
# import gc
# from collections import defaultdict
from typing import Type, List
from enum import Enum

class HideFlags(Enum):
    eNone = 0
    HideInHierarchy = 1
    HideInInspector = 2
    DontSaveInEditor = 4
    NotEditable = 8
    DontSaveInBuild = 0x10
    DontUnloadUnusedAsset = 0x20
    DontSave = 0x34
    HideAndDontSave = 0x3D


class __Object:

    __slots__ = ('m_CachedPtr')

    def __init__(self):
        self.m_CachedPtr = None

    def GetInstanceID(self):
        return self.m_CachedPtr.instanceID

    @property
    def instanceID(self):
        return self.GetInstanceID()

    def GetCachedPtr(self):
        return self.m_CachedPtr

    @property
    def cachedPtr(self):
        return self.m_CachedPtr

    @property
    def cpp(self):
        return self.m_CachedPtr

    @property
    def localIdentifierInFile(self)->int:
        return self.cpp.localIdentifierInFile
    @localIdentifierInFile.setter
    def localIdentifierInFile(self, value:int):
        self.cpp.localIdentifierInFile = value

    def __str__(self):
        return '{0}({1})'.format(self.name, self.__class__.__name__)


    def __IsNativeObjectAlive(self)->bool:
        return self.m_CachedPtr is not None

    # def __eq__(self, y:'Object')->bool:
    #     # assert(y is None or isinstance(y, Object))
    #     return Object.__CompareBaseObjects(self, y)

    # def __hash__(self):
    #     return self.instanceID

    @staticmethod
    def __CompareBaseObjects(lhs:'Object', rhs:'Object')->bool:
        flag1 = (lhs is None)
        flag2 = (rhs is None)
        if flag1 and flag2:
            return True
        if flag2:
            return not lhs.__IsNativeObjectAlive()
        if flag1:
            return not rhs.__IsNativeObjectAlive()
        return lhs.instanceID == rhs.instanceID

    def __IsNativeObjectAlive(self)->bool:
        return self.m_CachedPtr is not None
        
    # @staticmethod
    # def __IsNativeObjectAlive(o:'Object')->bool:
    #     if o.m_CachedPtr is None:
    #         return True
    #     if isinstance(o, Behaviour) or instance(o, ScriptableObject):
    #         return False
    #     return Object.__DoesObjectWithInstanceIDExist(o.m_InstancID)
    
    # @staticmethod
    # def __DoesObjectWithInstanceIDExist(instanceID:int)->bool:
    #     # TODO
    #     return self.m_CachedPtr is not None

    def Clean(self):
        pass

    @staticmethod
    def Destroy():
        pass

    @staticmethod
    def DestroyImmediate(obj):
        obj.Clean()
        # assert(obj.m_CachedPtr is not None)
        # FishEngineInternal.Object.Destroy(obj.m_CachedPtr)


    @staticmethod
    def FindObjectOfType(type)->'Object':
        return Object.FindObjectsOfType(type)[0]

    # ret: list of Object
    @staticmethod
    def FindObjectsOfType(type: Type['Object']) -> List['Object']:
    # def FindObjectsOfType(type: Type[Object]):
        from . import Script
        if issubclass(type, Script):
            return list(Script._Script__scripts[type])
        return FishEngineInternal.FindObjectsOfType(type.ClassID)


    @staticmethod
    def Instantiate(original: 'Object', parent: 'Transform' = None, instantiateInWorldSpace: bool = False):
        from . import GameObject
        if isinstance(original, GameObject):
            return original.Clone()
        return None


    @property
    def name(self):
        return self.m_CachedPtr.name
    @name.setter
    def name(self, name):
        self.m_CachedPtr.name = name

    def Serialize(self, dumper):
        # dumper.d('name', self.name)
        dumper.d('m_PrefabParentObject', None)
        dumper.d('m_PrefabInternal', None)
        pass

    def Deserialize(self, loader):
        pass



# def Object__new__(cls):
#     return FishEngineInternal.CreateObject()

# def Object__init__(self):
#     pass

Object = FishEngineInternal.Object
# Object.__new__ = Object__new__
# Object.__init__ = Object__init__
# Object.ClassID = FishEngineInternal.ObjectClassID()
Object.Serialize = __Object.Serialize
Object.Deserialize = __Object.Deserialize
Object.FindObjectsOfType = __Object.FindObjectsOfType