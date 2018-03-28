from . import Object
import FishEngineInternal

class __Prefab(Object):
    __slots__ = ('m_fileIDToObject', 'm_RootGameObject')
    def __init__(self):
        super().__init__()
        self.m_fileIDToObject = {}   # int->Object
        self.m_RootGameObject = None
       
    def FindObjectByFileID(self, fileID:int):
        if fileID not in self.m_fileIDToObject:
            print("ERROR: fileID not found:", fileID)
        return self.m_fileIDToObject[fileID]

    def Clean(self):
        del self.m_fileIDToObject
        del self.m_RootGameObject

    def Serialize(self, dumper):
        super(Prefab, self).Serialize(dumper)
        dumper.d('m_Modification', [])
        dumper.d('m_ParentPrefab', {'fileID': 1, 'guid': "fjpeoi"})
        dumper.d('m_IsPrefabParent', 0)

# def Prefab__new__(cls):
#     return FishEngineInternal.CreatePrefab()

# def Prefab__init__(self):
#     pass

Prefab = FishEngineInternal.Prefab
# Prefab.__new__ = Prefab__new__
# Prefab.__init__ = Prefab__init__
Prefab.ClassID = FishEngineInternal.PrefabClassID()
Prefab.Serialize = __Prefab.Serialize